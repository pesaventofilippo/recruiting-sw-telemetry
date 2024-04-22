#include "reader.h"
using namespace std;

queue<RawMessage> canQueue;
mutex canMutex;
condition_variable canFlag;


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
