#pragma once
#include <map>
#include <cstring>
#include "parser.h"
using namespace std;


// Clear all statistics
void clearStats();

// Clear statistics and reset session ID
void resetStats();

// Save statistics to a CSV file
void saveStats();

// Start logging messages to a file
void startLogging();

// Stop logging messages and rename the file
void stopLogging();

// Log a message to the log file and update statistics
void logMessage(const ParsedMessage &msg);
