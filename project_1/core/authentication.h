#pragma once
#include "common_imports.h"

extern bool authenticated;
extern User currentUser;
extern std::string username;
extern std::string password;


bool login();
bool logout();
