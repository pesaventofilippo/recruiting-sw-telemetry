#include "data_management.h"


bool readDataPoints(const std::string &filename, std::vector<DataPoint> &dataPoints) {
    std::ifstream file(filename);
    if (!file.is_open())
        return false;

    DataPoint point;
    while (file >> point.dist >> point.speed >> point.throttle >> point.brake >> point.gear)
        dataPoints.push_back(point);

    file.close();
    return true;
}


void loadDriver(const std::string &name, const ImVec4 &color) {
    std::string filename = "telemetry/" + name + ".csv";
    DriverData driver;
    driver.name = name;
    driver.color = color;
    if (readDataPoints(filename, driver.dataPoints)) {
        loadedDrivers.push_back(driver);
    }
}


void resetDrivers() {
    loadedDrivers.clear();
}
