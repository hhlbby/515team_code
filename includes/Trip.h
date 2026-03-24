#pragma once
#include <string>

class Trip {
public:
    int id;
    int bus_id;
    int route_id;
    std::string departure_date;
    std::string arrival_date;
    int passengers_count;
    double ticket_price;
    double total_cost;

    Trip(int _id, int bus, int route, const std::string& dep, const std::string& arr, int passengers, double price);
};