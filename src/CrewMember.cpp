#include "CrewMember.h"

CrewMember::CrewMember(int _id, const std::string& _tax_number, const std::string& _last_name,
                       const std::string& _first_name, const std::string& _patronymic, int _birth_year,
                       const std::string& _address, int _experience_years, const std::string& _category,
                       int _bus_id, int _user_id)
    : id(_id), tax_number(_tax_number), last_name(_last_name), first_name(_first_name),
      patronymic(_patronymic), birth_year(_birth_year), address(_address),
      experience_years(_experience_years), category(_category), bus_id(_bus_id), user_id(_user_id) {}