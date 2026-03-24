#include "Bus.h"

Bus::Bus(int _id, const std::string& _bus_number, const std::string& _model_name, int _total_mileage)
    : id(_id), bus_number(_bus_number), model_name(_model_name), total_mileage(_total_mileage) {}