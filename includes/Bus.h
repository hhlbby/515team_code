#pragma once
#include <string>

class Bus {
public:
    int id;
    std::string bus_number;
    std::string model_name;
    int total_mileage;

    Bus() = default;
    Bus(int _id, const std::string& _bus_number, const std::string& _model_name, int _total_mileage);
};