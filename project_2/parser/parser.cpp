#include "parser.h"
using namespace std;


ParsedMessage parseRawMessage(const RawMessage &msg) {
    // The string msg.message is in the format "ID#PAYLOAD":
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


bool isStartMessage(const ParsedMessage &msg) {
    // A start message is a message with ID 0x0A0 and payload 0x6601 or 0xFF01
    return msg.id == 0x0A0 && (msg.payload == 0x6601 || msg.payload == 0xFF01);
}


bool isStopMessage(const ParsedMessage &msg) {
    // A stop message is a message with ID 0x0A0 and payload 0x66FF
    return msg.id == 0x0A0 && msg.payload == 0x66FF;
}
