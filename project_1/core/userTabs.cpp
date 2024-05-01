#include "userTabs.h"
#include "data_management.h"
#include "plots.h"


void displayUserTabs() {
    ImGui::Begin("Data Plot");

    if (loadedDrivers.empty()) {
        ImGui::Text("No data loaded yet.");
    }
    else {
        plotSpeed();
        plotThrottle();
        plotBrake();
    }

    ImGui::End();
}


void displayMaintainerTabs() {
    ImGui::Begin("Style");

    if (loadedDrivers.empty()) {
        ImGui::Text("No data loaded yet.");
    }
    else {
        for (auto& driver : loadedDrivers) {
            ImGui::ColorEdit3(driver.name.c_str(), &driver.color.x);
            if (ImGui::Button("Reset")) {
                driver.color = ImVec4(1, 1, 1, 1);
            }
            ImGui::Separator();
        }
    }

    ImGui::End();
}


void displayAdminTabs() {
    ImGui::Begin("Manage Data");

    static std::string name;
    ImGui::InputText("Driver Identifier", &name);
    if (ImGui::Button("Load")) {
        loadDriver(name, ImVec4(1, 1, 1, 1));
    }

    ImGui::Separator();
    if (ImGui::Button("Reset All")) {
        resetDrivers();
    }

    ImGui::Separator();
    ImGui::Text("Remove driver: ");
    for (auto& driver : loadedDrivers) {
        ImGui::SameLine();
        if (ImGui::Button(driver.name.c_str())) {
            removeDriver(driver.name);
        }
    }

    ImGui::End();
}
