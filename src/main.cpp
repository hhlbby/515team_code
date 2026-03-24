#include <iostream>
#include "Database.h"
#include "User.h"
#include "Bus.h"
#include "CrewMember.h"
#include "Route.h"
#include "Trip.h"

int main() {
    Database db("../db/tourist_bureau.db");
    std::cout << "Database opened successfully!" << std::endl;
    std::string login, password;
    std::cout << "Enter login\n";
    std::cin >> login;
    std::cout << "Enter password\n";
    std::cin >> password;

    User* user = db.authenticateUser(login, password);
    if (!user) {
        std::cout << "Authentication failed! Check username/password." << std::endl;
        return 1;
    }
    if (!user) return 1;
    std::cout << "Welcome, " << user->full_name << " (" << user->role << ")" << std::endl;

    auto buses = db.getAllBuses();
    for (auto& bus : buses)
        std::cout << "Bus ID: " << bus.id << ", Number: " << bus.bus_number
                  << ", Model: " << bus.model_name << ", Mileage: " << bus.total_mileage << std::endl;

    if (!buses.empty()) {
        int bus_id = buses[0].id;
        auto crew = db.getCrewByBus(bus_id);
        for (auto& member : crew)
            std::cout << member.last_name << " " << member.first_name
                      << " (" << member.category << "), Tax #: " << member.tax_number << std::endl;
    }

    auto routes = db.getAllRoutes();
    for (auto& route : routes)
        std::cout << route.route_name << " (" << route.start_point << " -> "
                  << route.end_point << "), Distance: " << route.distance_km << " km" << std::endl;

    if (!buses.empty()) {
        int bus_id = buses[0].id;
        auto trips = db.getTripsByBus(bus_id);
        for (auto& trip : trips)
            std::cout << "Trip ID: " << trip.id << ", Route ID: " << trip.route_id
                      << ", Departure: " << trip.departure_date
                      << ", Arrival: " << trip.arrival_date
                      << ", Passengers: " << trip.passengers_count
                      << ", Ticket: " << trip.ticket_price
                      << ", Total: " << trip.total_cost << std::endl;
    }

    delete user;
    return 0;
}