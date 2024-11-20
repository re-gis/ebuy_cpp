#ifndef USER_H
#define USER_H

#include <string>
#include <ctime>
#include <crow.h>
#include <nlohmann/json.hpp> 

class User
{
public:
    int id;
    std::string username;
    std::string email;
    std::string password;
    std::string role;
    std::time_t created_at;
    std::time_t updated_at;

    nlohmann::json to_json() const
    {
        nlohmann::json json;
        json["id"] = id;
        json["username"] = username;
        json["email"] = email;
        json["role"] = role;
        json["created_at"] = static_cast<long long>(created_at); // Ensure time_t is converted to a valid format
        json["updated_at"] = static_cast<long long>(updated_at); // Same for updated_at
        return json;
    }
};

#endif