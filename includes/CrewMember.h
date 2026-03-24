#pragma once
#include <string>

class CrewMember {
public:
    int id;
    std::string tax_number;
    std::string last_name;
    std::string first_name;
    std::string patronymic;
    int birth_year;
    std::string address;
    int experience_years;
    std::string category;
    int bus_id;
    int user_id;

    CrewMember() = default;
    CrewMember(int _id, const std::string& _tax_number, const std::string& _last_name,
               const std::string& _first_name, const std::string& _patronymic, int _birth_year,
               const std::string& _address, int _experience_years, const std::string& _category,
               int _bus_id, int _user_id);
};