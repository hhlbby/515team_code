#include "Trip.h"

Trip::Trip(int _id, int bus, int route, const std::string& dep, const std::string& arr, int passengers, double price)
    : id(_id), bus_id(bus), route_id(route), departure_date(dep), arrival_date(arr),
      passengers_count(passengers), ticket_price(price), total_cost(passengers * price) {}