#include "User.h"

User::User(int _id, const std::string& _username, const std::string& _password,
           const std::string& _full_name, const std::string& _role)
    : id(_id), username(_username), password(_password), full_name(_full_name), role(_role) {}