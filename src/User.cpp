#include "User.h"

User::User(int _id, const std::string& uname, const std::string& pwd, const std::string& name, const std::string& r)
    : id(_id), username(uname), password_hash(pwd), full_name(name), role(r) {}