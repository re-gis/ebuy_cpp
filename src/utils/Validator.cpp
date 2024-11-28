#include <string>
#include "utils/Validator.hpp"
#include <crypt.h>
#include <iostream>

bool Validators::checkValidPassword(std::string &_storedPass, std::string _passToCheck)
{
    std::cout << std::string(_storedPass) << std::endl;
    std::cout << _passToCheck << std::endl;
    const char *salt = _storedPass.substr(0, 12).c_str();
    char *hashedInputPass = crypt(_passToCheck.c_str(), salt);

    if (std::string(_storedPass) == _passToCheck)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Validators::checkValidRole(std::string &_storedRole, std::string _roleToCheck)
{

    const char *salt = _storedRole.substr(0, 12).c_str();
    char *hashedRole = crypt(_roleToCheck.c_str(), salt);

    if (std::string(_storedRole) == _roleToCheck)
    {
        return true;
    }
    else
    {
        return false;
    }
}
