#include "app.h"
#include "loginTab.h"
#include "userTabs.h"
#include "authentication.h"
#include "common_imports.h"

// Global variables for the state
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
        // Display different tabs based on permissions.
        // If an user has role == x, they can also access tabs for role >= x.

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
        ImGui::Text("Logged in as: %s.", currentUser.username.c_str());
        ImGui::Separator();
        if (ImGui::Button("Logout")) {
            logout();
        }
    }

    ImGui::End();
}
