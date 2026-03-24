#include "Route.h"

Route::Route(int _id, const std::string& _route_name, const std::string& _start_point,
             const std::string& _end_point, double _distance_km)
    : id(_id), route_name(_route_name), start_point(_start_point), end_point(_end_point), distance_km(_distance_km) {}