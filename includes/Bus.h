#pragma once
#include <string>

class Bus {
public:
    int id;
    std::string bus_number;
    std::string model_name;
    int total_mileage;

    Bus(int _id, const std::string& number, const std::string& model, int mileage);
};