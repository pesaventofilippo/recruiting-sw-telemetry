#include "plots.h"
#include "data_management.h"


void plotSpeed() {
    if (ImPlot::BeginPlot("2024 Australian GP Quali - Speed", "Distance", "Speed")) {

        for (const auto &driver: loadedDrivers) {
            std::vector<float> x, y;
            for (const auto &point: driver.dataPoints) {
                x.push_back(point.dist);
                y.push_back(point.speed);
            }
            ImPlot::PushStyleColor(ImPlotCol_Line, driver.color);
            ImPlot::PlotLine(driver.name.c_str(), x.data(), y.data(), x.size());
            ImPlot::PopStyleColor();
        }

        ImPlot::EndPlot();
    }
}


void plotThrottle() {
    if (ImPlot::BeginPlot("2024 Australian GP Quali - Throttle", "Distance", "Throttle")) {

        for (const auto &driver: loadedDrivers) {
            std::vector<float> x, y;
            for (const auto &point: driver.dataPoints) {
                x.push_back(point.dist);
                y.push_back(point.throttle);
            }
            ImPlot::PushStyleColor(ImPlotCol_Line, driver.color);
            ImPlot::PlotLine(driver.name.c_str(), x.data(), y.data(), x.size());
            ImPlot::PopStyleColor();
        }

        ImPlot::EndPlot();
    }
}


void plotBrake() {
    if (ImPlot::BeginPlot("2024 Australian GP Quali - Brake", "Distance", "Brake")) {

        for (const auto &driver: loadedDrivers) {
            std::vector<float> x, y;
            for (const auto &point: driver.dataPoints) {
                x.push_back(point.dist);
                y.push_back(point.brake);
            }
            ImPlot::PushStyleColor(ImPlotCol_Line, driver.color);
            ImPlot::PlotLine(driver.name.c_str(), x.data(), y.data(), x.size());
            ImPlot::PopStyleColor();
        }

        ImPlot::EndPlot();
    }
}


void plotGear() {
    if (ImPlot::BeginPlot("2024 Australian GP Quali - Gear", "Distance", "Gear")) {

        for (const auto &driver: loadedDrivers) {
            std::vector<float> x, y;
            for (const auto &point: driver.dataPoints) {
                x.push_back(point.dist);
                y.push_back(point.gear);
            }
            ImPlot::PushStyleColor(ImPlotCol_Line, driver.color);
            ImPlot::PlotLine(driver.name.c_str(), x.data(), y.data(), x.size());
            ImPlot::PopStyleColor();
        }

        ImPlot::EndPlot();
    }
}
