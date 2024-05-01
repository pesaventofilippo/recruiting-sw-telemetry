#include "common_imports.h"
#include "app.h"


struct User {
    std::string username;
    std::string password;
    int role = 0;
};

std::vector<User> users = {
    {"admin", "password", 0},
    {"maintainer", "password", 1},
    {"user", "password", 2}
};

bool authenticated = false;
User currentUser;
std::string username;
std::string password;


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

std::vector<DriverData> loadedDrivers;


bool read_data_points(const std::string &filename, std::vector<DataPoint> &dataPoints) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        ImGui::Text("Failed to open file!");
        return false;
    }
    else {
        DataPoint point;
        while (file >> point.dist >> point.speed >> point.throttle >> point.brake >> point.gear) {
            dataPoints.push_back(point);
        }
    }

    file.close();
    return true;
}


void handle_login_attempt() {
    for (User &user : users) {
        if (user.username == username && user.password == password) {
            authenticated = true;
            currentUser = user;
            return;
        }
    }

    ImGui::Text("Invalid username or password!");
}


void reset_plots() {
    loadedDrivers.clear();
}


void add_driver_view(const std::string &name, const ImVec4 &color) {
    std::string filename = "telemetry/" + name + ".csv";
    DriverData driver;
    driver.name = name;
    driver.color = color;
    if (read_data_points(filename, driver.dataPoints)) {
        loadedDrivers.push_back(driver);
    }
}


void render_user_tabs() {
    ImGui::Text("+ User Actions");

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

void render_maintainer_tabs() {
    ImGui::Text("+ Maintainer Actions");
}

void render_admin_tabs() {
    ImGui::Text("+ Admin Actions");

    static std::string name;
    ImGui::Begin("Load Data");
    ImGui::InputText("Driver Identifier", &name);
    if (ImGui::Button("Load")) {
        add_driver_view(name, ImVec4(1, 0, 0, 1));
    }
    ImGui::Separator();
    if (ImGui::Button("Reset")) {
        reset_plots();
    }
    ImGui::End();
}


void logout() {
    authenticated = false;
    currentUser = {};
    username.clear();
    password.clear();
}


void app_render() {
    ImGui::Begin("Info");

    if (!authenticated) {
        ImGui::Text("Login to access the app.");

        // Display login form
        ImGui::Begin("Login");

        ImGui::InputText("Username", &username);
        ImGui::InputText("Password", &password, ImGuiInputTextFlags_Password);

        if (ImGui::Button("Login")) {
            handle_login_attempt();
        }

        ImGui::End();
    }
    else {
        // Display tabs based on user role
        if (currentUser.role <= 2) {
            render_user_tabs();
        }
        if (currentUser.role <= 1) {
            render_maintainer_tabs();
        }
        if (currentUser.role == 0) {
            render_admin_tabs();
        }

        ImGui::Text("Logged in as %s!", currentUser.username.c_str());
        if (ImGui::Button("Logout")) {
            logout();
        }
    }

    ImGui::End();
}
