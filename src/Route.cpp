#include "Route.h"

Route::Route(int _id, const std::string& name, const std::string& start, const std::string& end, double dist)
    : id(_id), route_name(name), start_point(start), end_point(end), distance_km(dist) {}