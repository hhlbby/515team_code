#pragma once
#include <string>

class Route {
public:
    int id;
    std::string route_name;
    std::string start_point;
    std::string end_point;
    double distance_km;

    Route() = default;
    Route(int _id, const std::string& _route_name, const std::string& _start_point,
          const std::string& _end_point, double _distance_km);
};