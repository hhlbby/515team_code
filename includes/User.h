#pragma once
#include <string>

class User {
public:
    int id;
    std::string username;
    std::string password_hash;
    std::string full_name;
    std::string role;

    User(int _id, const std::string& uname, const std::string& pwd, const std::string& name, const std::string& r);
};