#include "Bus.h"

Bus::Bus(int _id, const std::string& number, const std::string& model, int mileage)
    : id(_id), bus_number(number), model_name(model), total_mileage(mileage) {}