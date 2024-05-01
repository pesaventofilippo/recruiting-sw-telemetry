#pragma once
#include "common_imports.h"

extern std::vector<DriverData> loadedDrivers;


void loadDriver(const std::string &name, const ImVec4 &color);


void resetDrivers();
