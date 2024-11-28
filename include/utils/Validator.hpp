#ifndef UTILS_VALIDATORS_HPP
#define UTILS_VALIDATORS_HPP

#include <string>

class Validators
{
public:
    Validators() {}
    bool checkValidPassword(std::string &_storedPassword, std::string _passToCheck);

    bool checkValidRole(std::string &_hashedRole, std::string _roleToCheck);
};

#endif
