#include <map>
#include <queue>
#include <mutex>
#include <thread>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <chrono>
#include <condition_variable>
using namespace std;

extern "C" {
    #include "fake_receiver.h"
}


struct RawMessage {
    char message[MAX_CAN_MESSAGE_SIZE];
    uint64_t timestamp;
};

struct ParsedMessage {
    uint16_t id;
    uint64_t payload;
    uint64_t timestamp;
};

queue<RawMessage> canQueue;  // Shared queue to hold messages to parse
mutex canMutex;              // Lock to prevent race conditions
condition_variable canFlag;  // Used to notify thread when new messages are available

enum STATE { IDLE, RUN };   // The two states for the state machine
STATE state = IDLE;         // State for the state machine
FILE* logFile = NULL;       // File to log messages
int sessionID = 0;          // Session ID to save different sessions to different files

map<uint16_t, uint64_t> messageCounts;      // Map message IDs to their count
map<uint16_t, uint64_t> firstReceivedAt;    // Map message IDs to their first received timestamp
map<uint16_t, uint64_t> lastReceivedAt;     // Map message IDs to their last received timestamp


uint64_t msTimestamp() {
    // Return the current unix timestamp in milliseconds
    using namespace chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}


void canReaderLoop() {
    char message[MAX_CAN_MESSAGE_SIZE];
    while (true) {
        int bytesRead = can_receive(message);
        if (bytesRead > 0) {
            // Save the message and the timestamp
            RawMessage canMsg;
            strncpy(canMsg.message, message, MAX_CAN_MESSAGE_SIZE);
            canMsg.timestamp = msTimestamp();

            // Lock the queue for writing
            unique_lock<mutex> lock(canMutex);
            canQueue.push(canMsg);
            lock.unlock();

            // Notify that a new message is present
            canFlag.notify_one();
        }
    }
}


RawMessage waitForMessage() {
    // Wait until the queue contains a message (using the flag).
    // The lock is released while waiting, and only re-acquired when needed.
    // -> THIS FUNCTION IS BLOCKING until a message arrives
    unique_lock<mutex> lock(canMutex);
    canFlag.wait(lock, []{ return !canQueue.empty(); });

    // Get the first message and release the lock
    RawMessage rawMessage = canQueue.front();
    canQueue.pop();
    lock.unlock();

    return rawMessage;
}


ParsedMessage parseRawMessage(const RawMessage &msg) {
    // The string canMsg.message is in the format "ID#PAYLOAD":
    // - ID is expressed in hexadecimal, and is at most 12 bits long (can be less)
    // - PAYLOAD is expressed in hexadecimal, and is at most 8 bytes (can be less)
    // For example, the payload "6601" is a 2-byte payload, where the first byte is 0x66 and the second byte is 0x01

    ParsedMessage parsedMsg;
    parsedMsg.timestamp = msg.timestamp;

    // Split the message into two strings using '#'
    string message(msg.message);
    size_t idDelimiter = message.find('#');
    string idStr = message.substr(0, idDelimiter);
    string payloadStr = message.substr(idDelimiter + 1);

    // Convert id and payload to integers
    parsedMsg.id = stoi(idStr, nullptr, 16);
    parsedMsg.payload = stoull(payloadStr, nullptr, 16);

    return parsedMsg;
}


void saveStats(const char* FILE_PATH) {
    FILE* file = fopen(FILE_PATH, "w");

    // Write statistics in CSV format
    fprintf(file, "ID,number_of_messages,mean_time\n");
    for (auto it = messageCounts.begin(); it != messageCounts.end(); it++) {
        uint16_t id = it->first;
        uint64_t count = it->second;
        double avgTime = (lastReceivedAt[id] - firstReceivedAt[id]) / (double)count;
        fprintf(file, "%03X,%llu,%f\n", id, count, avgTime);
    }

    fclose(file);
}


void startSession() {
    state = RUN;

    // Open the log file using the session ID (.tmp until done)
    string filename = "session_" + to_string(sessionID) + ".log.tmp";
    logFile = fopen(filename.c_str(), "w");

    // Reset the statistics
    messageCounts.clear();
    firstReceivedAt.clear();
    lastReceivedAt.clear();
}


void stopSession() {
    state = IDLE;

    // Close the log file and rename it
    fclose(logFile);
    logFile = NULL;
    string oldName = "session_" + to_string(sessionID) + ".log.tmp";
    string newName = "session_" + to_string(sessionID) + ".log";
    rename(oldName.c_str(), newName.c_str());

    // Save the statistics
    string statsFile = "stats_" + to_string(sessionID) + ".csv";
    saveStats(statsFile.c_str());

    // Increment the session ID
    sessionID++;
}


void handleMessage(const ParsedMessage &msg) {
    const char* LOG_FORMAT = "%llu\t0x%03X\t0x%X\n"; // As readme: "(%llu) %03X#%X\n"

    // Log the message to the log file
    fprintf(logFile, LOG_FORMAT, msg.timestamp, msg.id, msg.payload);

    // Update statistics
    messageCounts[msg.id]++;
    if (firstReceivedAt.find(msg.id) == firstReceivedAt.end())
        firstReceivedAt[msg.id] = msg.timestamp;
    lastReceivedAt[msg.id] = msg.timestamp;
}


int main() {
    const char* CAN_DUMP_PATH = "candump.log";

    // Open the CAN "interface"
    if (open_can(CAN_DUMP_PATH) != 0) {
        cerr << "Failed to open CAN interface: " << CAN_DUMP_PATH << endl;
        return -1;
    }

    // Start the reader thread
    thread readerThread(canReaderLoop);

    while (true) {
        // Wait for a new message to arrive
        RawMessage rawMsg = waitForMessage();

        // Parse the message
        ParsedMessage parsedMsg = parseRawMessage(rawMsg);

        // Check for a START message
        if (parsedMsg.id == 0x0A0 && (parsedMsg.payload == 0x6601 || parsedMsg.payload == 0xFF01)) {
            if (state == IDLE)
                startSession();
        }

        // If state is RUN, log the message + update stats
        if (state == RUN)
            handleMessage(parsedMsg);

        // Check for a STOP message
        // This is done after handling so that the stop message is also handled
        if (parsedMsg.id == 0x0A0 && parsedMsg.payload == 0x66FF) {
            if (state == RUN)
                stopSession();
        }
    }

    readerThread.join();
    close_can();
    return 0;
}
