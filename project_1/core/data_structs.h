#pragma once
#include "common_imports.h"

struct DataPoint {
    float dist;
    int speed;
    int throttle;
    int brake;
    int gear;
};

struct DriverData {
    std::string name;
    std::vector<DataPoint> dataPoints;
    ImVec4 color;
};

struct User {
    std::string username;
    std::string password;
    int role = 0;
};
