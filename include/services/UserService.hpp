#ifndef USERSERVICE_HPP
#define USERSERVICE_HPP

#include <string>
#include <vector>
#include <memory>
#include <odb/pgsql/database.hxx>
#include "../models/User.hpp"

class UserService
{
private:
    std::shared_ptr<odb::pgsql::database> db_;

    std::string hashPassword(const std::string &password);
    std::string hashAnyString(const std::string &_mystring);

public:
    explicit UserService(const std::shared_ptr<odb::pgsql::database> &db) : db_(db) {}

    std::string registerUser(const std::string &username, const std::string &email, const std::string &password, const std::string &role);
    std::string login(const std::string &email, const std::string &password);
    std::string updateUser(int user_id, const std::string &username, const std::string &email, const std::string &password);
    User getUserById(int user_id);
    std::vector<User> getAllUsersByRole(const std::string &role);
    std::string deleteUser(int userId);
    User getLoggedInUser(int userId);
};

#endif
