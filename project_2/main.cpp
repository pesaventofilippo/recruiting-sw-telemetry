#include <thread>
#include <iostream>
#include "reader.h"
#include "parser.h"
#include "statistics.h"
using namespace std;

extern "C" {
    #include "fake_receiver.h"
}

enum STATE { IDLE, RUN };  // The two states for the state machine
STATE state = IDLE;        // Current state


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


void startSession() {
    state = RUN;

    // Clear the statistics
    clearStats();

    // Open the log file using the session ID (.tmp until done)
    startLogging();
}


void stopSession() {
    state = IDLE;

    // Close the log file and rename it
    stopLogging();

    // Save the statistics
    saveStats();
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
        ParsedMessage msg = parseRawMessage(rawMsg);

        // Check for a START message
        if (isStartMessage(msg)) {
            if (state == IDLE)
                startSession();
        }

        // If state is RUN, log the message + update stats
        if (state == RUN)
            logMessage(msg);

        // Check for a STOP message
        // This is done after handling so that the stop message is also handled
        if (isStopMessage(msg)) {
            if (state == RUN)
                stopSession();
        }
    }

    readerThread.join();
    close_can();
    return 0;
}
