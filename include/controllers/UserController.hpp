#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H

#include <crow.h>
#include <string>
#include "services/UserService.hpp"

class UserController
{
public:
    UserController(UserService &service) : userService(service) {};

    void registerUserRoutes(crow::SimpleApp &app);
    void loginUserRoutes(crow::SimpleApp &app);
    void updateUserRoutes(crow::SimpleApp &app);
    void getUserRoutes(crow::SimpleApp &app);
    void getAllUsersByRoleRoutes(crow::SimpleApp &app);
    void deleteUserRoutes(crow::SimpleApp &app);

private:
    UserService &userService;
};

#endif