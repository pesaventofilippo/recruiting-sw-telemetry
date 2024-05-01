#include "userTabs.h"
#include "data_management.h"


void displayUserTabs() {
    ImGui::Begin("Data Plot");
    if (loadedDrivers.empty()) {
        ImGui::Text("No data loaded yet.");
    }
    else {
        ImPlot::BeginPlot("Throttle vs Distance", "Distance", "Throttle");
        for (const auto& driver : loadedDrivers) {
            std::vector<float> x, y;
            for (const auto& point : driver.dataPoints) {
                x.push_back(point.dist);
                y.push_back(point.speed);
            }
            ImPlot::PushStyleColor(ImPlotCol_Line, driver.color);
            ImPlot::PlotLine(driver.name.c_str(), x.data(), y.data(), x.size());
            ImPlot::PopStyleColor();
        }
        ImPlot::EndPlot();
    }

    ImGui::End();
}


void displayMaintainerTabs() {
}


void displayAdminTabs() {
    static std::string name;
    ImGui::Begin("Load Data");
    ImGui::InputText("Driver Identifier", &name);
    if (ImGui::Button("Load")) {
        loadDriver(name, ImVec4(1, 0, 0, 1));
    }
    ImGui::Separator();
    if (ImGui::Button("Reset")) {
        resetDrivers();
    }
    ImGui::End();
}
