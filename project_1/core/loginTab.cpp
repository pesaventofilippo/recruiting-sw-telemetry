#include "loginTab.h"
#include "authentication.h"


void displayLoginTab() {
    ImGui::Begin("Login");

    ImGui::InputText("Username", &username);
    ImGui::InputText("Password", &password, ImGuiInputTextFlags_Password);

    if (ImGui::Button("Login")) {
        login();
    }

    ImGui::End();
}
