#pragma once
#include <string>

class Route {
public:
    int id;
    std::string route_name;
    std::string start_point;
    std::string end_point;
    double distance_km;

    Route(int _id, const std::string& name, const std::string& start, const std::string& end, double dist);
};