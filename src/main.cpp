#include "database/DatabaseConfig.hpp"
#include "crow.h"
#include <iostream>
#include "services/UserService.hpp"
#include "controllers/UserController.hpp"

int main()
{

    const std::string db_user = "postgres";
    const std::string db_password = "Password@2001";
    const std::string db_host = "127.0.0.1";
    const std::string db_name = "ec_cpp";
    const unsigned int db_port = 5432;

    DatabaseConfig dbConfig(db_name, db_password, db_name, db_host, db_port);
    auto db = dbConfig.getDatabase();

    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
    ([]()
     { return "Welcome to the eCommerce API..."; });

    CROW_ROUTE(app, "/db_status")
    ([&db]()
     {
        if(db){
            return "Database is live...";
        }else{
            return "Database connection failed...";
        } });

    UserService userService(db);

    UserController userController(userService);
    userController.registerUserRoutes(app);
    userController.loginUserRoutes(app);
    userController.updateUserRoutes(app);
    userController.getUserRoutes(app);
    userController.getAllUsersByRoleRoutes(app);
    userController.deleteUserRoutes(app);

    app.port(8080).multithreaded().run();

    return 0;
}