#include "data_management.h"


bool isDriverLoaded(const std::string &name) {
    // Return if a driver is already loaded
    for (const auto &driver : loadedDrivers) {
        if (driver.name == name)
            return true;
    }
    return false;
}


bool readDataPoints(const std::string &filename, std::vector<DataPoint> &dataPoints) {
    // Read the data points for a driver from a given file
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
    // Load driver data from a telemetry file
    std::string filename = "telemetry/" + name + ".csv";
    DriverData driver;
    driver.name = name;
    driver.color = color;
    if (readDataPoints(filename, driver.dataPoints) && !isDriverLoaded(name)) {
        loadedDrivers.push_back(driver);
    }
}


void removeDriver(const std::string &name) {
    // Remove driver from loadedDrivers (if it exists)
    if (isDriverLoaded(name)) {
        loadedDrivers.erase(
            std::remove_if(loadedDrivers.begin(), loadedDrivers.end(),
                [name](const DriverData &driver) {
                    return driver.name == name;
                }),
            loadedDrivers.end());
    }
}


void resetDrivers() {
    // Remove all drivers
    loadedDrivers.clear();
}
