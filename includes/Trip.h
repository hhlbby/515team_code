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

    Trip() = default;
    Trip(int _id, int _bus_id, int _route_id, const std::string& _departure_date,
         const std::string& _arrival_date, int _passengers_count,
         double _ticket_price, double _total_cost);
};