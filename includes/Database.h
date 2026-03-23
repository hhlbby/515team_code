#pragma once
#include <sqlite3.h>
#include <string>
#include <vector>
#include "User.h"
#include "Bus.h"

class Database {
private:
    sqlite3* db;
    bool openDatabase(const std::string& filename);
public:
    Database(const std::string& filename);
    ~Database();

    std::vector<User> getAllUsers();
    std::vector<Bus> getAllBuses();
    User* authenticateUser(const std::string& username, const std::string& password);
};