#include "CrewMember.h"

CrewMember::CrewMember(int _id, const std::string& tax, const std::string& ln, const std::string& fn,
                       const std::string& pat, int birth, const std::string& addr, int exp,
                       const std::string& cat, int bus, int user)
    : id(_id), tax_number(tax), last_name(ln), first_name(fn), patronymic(pat),
      birth_year(birth), address(addr), experience_years(exp), category(cat),
      bus_id(bus), user_id(user) {}