#pragma once
#include <string>

class User {
public:
    int id;
    std::string username;
    std::string password;
    std::string full_name;
    std::string role;

    User() = default;
    User(int _id, const std::string& _username, const std::string& _password,
         const std::string& _full_name, const std::string& _role);
};