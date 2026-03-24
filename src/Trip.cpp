#include "Trip.h"

Trip::Trip(int _id, int _bus_id, int _route_id, const std::string& _departure_date,
           const std::string& _arrival_date, int _passengers_count,
           double _ticket_price, double _total_cost)
    : id(_id), bus_id(_bus_id), route_id(_route_id), departure_date(_departure_date),
      arrival_date(_arrival_date), passengers_count(_passengers_count),
      ticket_price(_ticket_price), total_cost(_total_cost) {}