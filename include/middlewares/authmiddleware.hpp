#ifndef AUTH_MIDDLEWARE_HPP
#define AUTH_MIDDLEWARE_HPP

#include <string>
#include <crow.h>
#include "models/User.hpp"
#include <nlohmann/json.hpp>

class AuthMiddleware
{
public:
    static bool isLoggedIn(const std::string &authorizationHeader);

    static nlohmann::json getLoggedInUser(const std::string &header);
};

#endif
