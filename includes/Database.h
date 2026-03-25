#pragma once
#include <vector>
#include <tuple>
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

public:
    Database(const std::string& filename);
    ~Database();

    bool openDatabase(const std::string& filename);
    bool execute(const std::string& sql);

    User* authenticateUser(const std::string& username, const std::string& password);

    std::vector<Bus> getAllBuses();
    std::vector<Route> getAllRoutes();
    std::vector<CrewMember> getCrewByBus(int bus_id);
    std::vector<Trip> getTripsByBus(int bus_id);

    void addCrewMember(
        const std::string& tax,
        const std::string& last,
        const std::string& first,
        const std::string& category,
        int bus_id,
        int birth_year,
        int experience,
        const std::string& address
    );

    void updateBusMileage(int bus_id, int mileage);
    std::vector<Trip> tripsByBusPeriod(int bus_id,const std::string& from,const std::string& to);

    std::tuple<int,int,double> statsByBus(int bus_id);

    std::vector<std::pair<int,double>> crewMoneyPeriod(const std::string& from,const std::string& to,double percent);

    std::vector<std::tuple<int,int,double>> mostExpensiveRoute();

    std::tuple<int,int,int> topBusStats();

    double crewMoneyOnDate(int crew_id,const std::string& date,double percent);
    void deleteCrewMember(int id);
};