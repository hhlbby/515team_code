#pragma once
#include <vector>
#include <string>
#include <sqlite3.h>
#include "User.h"
#include "Bus.h"
#include "CrewMember.h"
#include "Route.h"
#include "Trip.h"

class Database {
private:
    sqlite3* db;
    bool openDatabase(const std::string& filename);
public:
    Database(const std::string& filename);
    ~Database();
    bool execute(const std::string& sql);
    std::vector<User> getAllUsers();
    User* authenticateUser(const std::string& username, const std::string& password);
    std::vector<Bus> getAllBuses();
    std::vector<CrewMember> getCrewByBus(int bus_id);
    std::vector<Route> getAllRoutes();
    std::vector<Trip> getTripsByBus(int bus_id);
    void updateBusMileage(int bus_id, int new_mileage);
};