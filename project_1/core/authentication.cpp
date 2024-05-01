#include "authentication.h"

const std::vector<User> USERS = {
    {"admin", "password", 0},
    {"maintainer", "password", 1},
    {"user", "password", 2}
};


bool login() {
    for (const User &user : USERS) {
        if (user.username == username && user.password == password) {
            authenticated = true;
            currentUser = user;
        }
    }
    return authenticated;
}


bool logout() {
    authenticated = false;
    currentUser = {};
    username.clear();
    password.clear();
    return true;
}
