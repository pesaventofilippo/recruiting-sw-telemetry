#pragma once
#include <cstdio>
#include <cstring>
#include <iostream>
#include "reader.h"
using namespace std;


struct ParsedMessage {
    uint16_t id;
    uint64_t payload;
    uint64_t timestamp;
};


ParsedMessage parseRawMessage(const RawMessage &msg);
