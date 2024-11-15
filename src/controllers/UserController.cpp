#include "controllers/UserController.hpp"
#include "responses/ResponseUtils.hpp"

void UserController::registerUserRoutes(crow::SimpleApp &app)
{
    CROW_ROUTE(app, "/register").methods("POST"_method)([this](const crow::request &req)
    {
        try {
            auto json = crow::json::load(req.body);
            std::string username = json["username"].s();
            std::string email = json["email"].s();
            std::string password = json["password"].s();
            std::string role = json["role"].s();

            if (username.empty() || email.empty() || password.empty()) {
                return crow::response(ResponseUtils::createResponse(false, "All user details are required"));
            }

            if (role.empty()) {
                role = "CUSTOMER";  // Default role
            }

            std::string result = userService.registerUser(username, email, password, role);
            return crow::response(ResponseUtils::createResponse(true, "User registered successfully", result));
        } catch (const std::exception &e) {
            return crow::response(ResponseUtils::createResponse(false, std::string("Error: ") + e.what()));
        }
    });
}

void UserController::loginUserRoutes(crow::SimpleApp &app)
{
    CROW_ROUTE(app, "/login").methods("POST"_method)([this](const crow::request &req)
    {
        try {
            auto json = crow::json::load(req.body);
            std::string email = json["email"].s();
            std::string password = json["password"].s();
            
            if (email.empty() || password.empty()) {
                return crow::response(ResponseUtils::createResponse(false, "Login details are required"));
            }

            std::string token = userService.login(email, password);
            if (token.find("Invalid") != std::string::npos) {
                return crow::response(ResponseUtils::createResponse(false, token));
            }

            return crow::response(ResponseUtils::createResponse(true, "Login successful", token));
        } catch (const std::exception &e) {
            return crow::response(ResponseUtils::createResponse(false, std::string("Error: ") + e.what()));
        }
    });
}

void UserController::updateUserRoutes(crow::SimpleApp &app)
{
    CROW_ROUTE(app, "/update/<int>").methods("PUT"_method)([this](const crow::request &req, int _user_id)
    {
        try {
            auto json = crow::json::load(req.body);
            std::string username = json["username"].s();
            std::string email = json["email"].s();
            std::string password = json["password"].s();

            std::string result = userService.updateUser(_user_id, username, email, password);
            return crow::response(ResponseUtils::createResponse(true, "User updated successfully", result));
        } catch (const std::exception &e) {
            return crow::response(ResponseUtils::createResponse(false, std::string("Error: ") + e.what()));
        }
    });
}

void UserController::getUserRoutes(crow::SimpleApp &app)
{
    CROW_ROUTE(app, "/user/<int>").methods("GET"_method)([this](int _user_id)
    {
        try {
            User user = userService.getUserById(_user_id);
            crow::json::wvalue response;
            response["id"] = user.id;
            response["username"] = user.username;
            response["email"] = user.email;
            response["role"] = user.role;
            return crow::response(response);
        } catch (const std::exception &e) {
            return crow::response(ResponseUtils::createResponse(false, std::string("Error: ") + e.what()));
        }
    });
}

void UserController::getAllUsersByRoleRoutes(crow::SimpleApp &app)
{
    CROW_ROUTE(app, "/users/role/<string>").methods("GET"_method)([this](const crow::request &req, const std::string &role)
    {
        try {
            std::vector<User> users = userService.getAllUsersByRole(role);

            // Create an empty wvalue object (this will hold our array)
            crow::json::wvalue response_json;

            // Loop through users and add each user's JSON representation to the response
            for (const auto& user : users) {
                response_json[user.id] = user.to_json();  // Use an object-based approach to hold the users
            }

            // Return the response as a JSON
            return crow::response(response_json);
        } catch (const std::exception &e) {
            return crow::response(ResponseUtils::createResponse(false, std::string("Error: ") + e.what()));
        }
    });
}




void UserController::deleteUserRoutes(crow::SimpleApp &app)
{
    CROW_ROUTE(app, "/user/<int>").methods("DELETE"_method)([this](int _user_id)
    {
        try {
            std::string result = userService.deleteUser(_user_id);
            return crow::response(ResponseUtils::createResponse(true, "User deleted successfully", result));
        } catch (const std::exception &e) {
            return crow::response(ResponseUtils::createResponse(false, std::string("Error: ") + e.what()));
        }
    });
}
