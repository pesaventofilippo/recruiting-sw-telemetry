#include "statistics.h"
using namespace std;


const char* LOG_FORMAT = "%llu\t%03X\t%X\n"; // As readme: "(%llu) %03X#%X\n"
int sessionID = 0;      // Session ID to save different sessions to different files
FILE* logFile = NULL;   // File to log messages

map<uint16_t, uint64_t> messageCounts;      // Map message IDs to their count
map<uint16_t, uint64_t> firstReceivedAt;    // Map message IDs to their first received timestamp
map<uint16_t, uint64_t> lastReceivedAt;     // Map message IDs to their last received timestamp


// Generate the log file path based on the session ID
string getLogFilePath(bool isTmp=false) {
    string filename = "session_" + to_string(sessionID) + ".log";
    if (isTmp)
        filename += ".tmp";
    return filename;
}


// Generate the statistics file path based on the session ID
string getStatsFilePath() {
    return "stats_" + to_string(sessionID) + ".csv";
}


void clearStats() {
    messageCounts.clear();
    firstReceivedAt.clear();
    lastReceivedAt.clear();
}


void resetStats() {
    clearStats();
    sessionID = 0;
}


void saveStats() {
    FILE* file = fopen(getStatsFilePath().c_str(), "w");

    // Write statistics in CSV format
    fprintf(file, "ID,number_of_messages,mean_time_ms\n");
    for (auto it = messageCounts.begin(); it != messageCounts.end(); it++) {
        uint16_t id = it->first;
        uint64_t count = it->second;
        double avgTime = (lastReceivedAt[id] - firstReceivedAt[id]) / (double)count;
        fprintf(file, "%03X,%llu,%f\n", id, count, avgTime);
    }

    fclose(file);
}


void startLogging() {
    // Open the log file using the session ID (.tmp until done)
    logFile = fopen(getLogFilePath(true).c_str(), "w");
    fprintf(logFile, "TIMESTAMP_MS\tID\tPAYLOAD\n");
}


void stopLogging() {
    // Close the log file
    fclose(logFile);
    logFile = NULL;

    // Rename the file removing the .tmp placeholder
    string oldName = getLogFilePath(true);
    string newName = getLogFilePath(false);
    rename(oldName.c_str(), newName.c_str());

    // Increment the session ID
    sessionID++;
}


void logMessage(const ParsedMessage &msg) {
    // Log the message to the log file
    fprintf(logFile, LOG_FORMAT, msg.timestamp, msg.id, msg.payload);

    // Update statistics
    messageCounts[msg.id]++;
    if (firstReceivedAt.find(msg.id) == firstReceivedAt.end())
        firstReceivedAt[msg.id] = msg.timestamp;
    lastReceivedAt[msg.id] = msg.timestamp;
}
