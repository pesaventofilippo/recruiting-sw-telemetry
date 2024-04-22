#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include "timestamp.h"
using namespace std;

extern "C" {
    #include "fake_receiver.h"
}


struct RawMessage {
    char message[MAX_CAN_MESSAGE_SIZE];
    uint64_t timestamp;
};

// Use extern so that the variables are shared between files
extern queue<RawMessage> canQueue;  // Shared queue to hold messages to parse
extern mutex canMutex;              // Lock to prevent race conditions
extern condition_variable canFlag;  // Used to notify thread when new messages are available


void canReaderLoop();
