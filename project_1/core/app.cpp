#include "common_imports.h"
#include "app.h"
#include "authentication.h"
#include "loginTab.h"
#include "userTabs.h"

bool authenticated = false;
User currentUser;
std::string username;
std::string password;

std::vector<DriverData> loadedDrivers;


void app_render() {
    ImGui::Begin("Info");

    if (!authenticated) {
        ImGui::Text("Login to access the app.");
        displayLoginTab();
    }
    else {
        ImGui::Text("Permissions:");

        if (currentUser.role <= 2) {
            ImGui::Text("+ User");
            displayUserTabs();
        }
        if (currentUser.role <= 1) {
            ImGui::Text("+ Maintainer");
            displayMaintainerTabs();
        }
        if (currentUser.role == 0) {
            ImGui::Text("+ Admin");
            displayAdminTabs();
        }

        ImGui::Separator();
        ImGui::Text("Logged in as %s!", currentUser.username.c_str());
        ImGui::Separator();
        if (ImGui::Button("Logout")) {
            logout();
        }
    }

    ImGui::End();
}
