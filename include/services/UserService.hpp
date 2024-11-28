#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include <string>
#include <pqxx/pqxx>
#include "models/User.hpp"
#include <crypt.h>

class UserService
{
public:
    pqxx::connection &conn;

    UserService(pqxx::connection &conn) : conn(conn) {}
    std::string registerUser(const std::string &username, const std::string &email, const std::string &password, const std::string &role);
    std::string login(const std::string &email, const std::string &password);
    std::string updateUser(int user_id, std::string &username, std::string &email, std::string &password);
    static User getUserById(int user_id);
    std::vector<User> getAllUsers();
    std::string deleteUser(int user_id);
    User getLoggedInUser(int user_id);
    std::vector<User> getAllUsersByRole(const std::string &role);

private:
    std::string hashPassword(const std::string &password);
    std::string generateToken(int user_id, const std::string &email, const std::string &role, std::string &username);
    std::string hashAnyString(const std::string &_mystring);
};

#endif