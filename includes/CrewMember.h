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

    CrewMember(int _id, const std::string& tax, const std::string& ln, const std::string& fn,
               const std::string& pat, int birth, const std::string& addr, int exp,
               const std::string& cat, int bus, int user);
};