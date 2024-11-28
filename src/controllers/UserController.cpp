#include "controllers/UserController.hpp"
#include "responses/ResponseUtils.hpp"
#include <nlohmann/json.hpp>
#include "middlewares/authmiddleware.hpp"

void UserController::registerUserRoutes(crow::SimpleApp &app)
{
    CROW_ROUTE(app, "/register").methods("POST"_method)([this](const crow::request &req)
                                                        {
        try {
            auto json = nlohmann::json::parse(req.body);
            std::string username = json["username"];
            std::string email = json["email"];
            std::string password = json["password"];
            std::string role = json.value("role", "CUSTOMER");  // Default role to CUSTOMER

            if (username.empty() || email.empty() || password.empty()) {
                return crow::response(ResponseUtils::createResponse(false, "All user details are required"));
            }

            std::string result = userService.registerUser(username, email, password, role);
            return crow::response(ResponseUtils::createResponse(true, result));
        } catch (const std::exception &e) {
            return crow::response(ResponseUtils::createResponse(false, std::string("Error: ") + e.what()));
        } });
}

void UserController::loginUserRoutes(crow::SimpleApp &app)
{
    CROW_ROUTE(app, "/login").methods("POST"_method)([this](const crow::request &req)
                                                     {
        try {
            auto json = nlohmann::json::parse(req.body);  // Use lohmann/json for parsing
            std::string email = json["email"];
            std::string password = json["password"];

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
        } });
}

void UserController::updateUserRoutes(crow::SimpleApp &app)
{
    CROW_ROUTE(app, "/update/<int>").methods("PUT"_method)([this](const crow::request &req, int _user_id)
                                                           {
        try {
            auto json = nlohmann::json::parse(req.body);  // Use lohmann/json for parsing
            std::string username = json["username"];
            std::string email = json["email"];
            std::string password = json["password"];

            std::string result = userService.updateUser(_user_id, username, email, password);
            return crow::response(ResponseUtils::createResponse(true, "User updated successfully", result));
        } catch (const std::exception &e) {
            return crow::response(ResponseUtils::createResponse(false, std::string("Error: ") + e.what()));
        } });
}

void UserController::getUserRoutes(crow::SimpleApp &app)
{
    CROW_ROUTE(app, "/user/<int>").methods("GET"_method)([this](const crow::request &req, int _user_id)
                                                         {
        // Extract the Authorization header
        std::string auth_header = req.get_header_value("Authorization");

        // Check if the token is missing or invalid
        if (auth_header.empty() || !AuthMiddleware::isLoggedIn(auth_header)) {
            return crow::response(ResponseUtils::createResponse(false, "Invalid or missing token"));  // Return immediately, response is already set
        }

        // If authorized, proceed with fetching the user
        try {
            User user = userService.getUserById(_user_id);

            nlohmann::json response;
            response["id"] = user.id;
            response["username"] = user.username;
            response["email"] = user.email;
            response["role"] = user.role;

            return crow::response(response.dump());  // Return the response directly
        } catch (const std::exception &e) {
            return crow::response(ResponseUtils::createResponse(false, std::string("Error: ") + e.what()));
        } });
}

void UserController::getAllUsersByRoleRoutes(crow::SimpleApp &app)
{
    CROW_ROUTE(app, "/users/role/<string>").methods("GET"_method)([this](const crow::request &req, const std::string &role)
                                                                  {

                                                                    // Extract the Authorization header
        std::string auth_header = req.get_header_value("Authorization");

        // Check if the token is missing or invalid
        if (auth_header.empty() || !AuthMiddleware::isLoggedIn(auth_header)) {
            return crow::response(ResponseUtils::createResponse(false, "Invalid or missing token"));  // Return immediately, response is already set
        }

        try {
            std::vector<User> users = userService.getAllUsersByRole(role);

            nlohmann::json response_json;

            for (const auto& user : users) {
                response_json[std::to_string(user.id)] = user.to_json();  // lohmann/json serialization
            }

            return crow::response(response_json.dump());  // Serialize the response to JSON
        } catch (const std::exception &e) {
            return crow::response(ResponseUtils::createResponse(false, std::string("Error: ") + e.what()));
        } });
}

void UserController::deleteUserRoutes(crow::SimpleApp &app)
{
    CROW_ROUTE(app, "/user/<int>").methods("DELETE"_method)([this](const crow::request &req, int _user_id)
                                                            {

                                                                // Extract the Authorization header
        std::string auth_header = req.get_header_value("Authorization");

        // Check if the token is missing or invalid
        if (auth_header.empty() || !AuthMiddleware::isLoggedIn(auth_header)) {
            return crow::response(ResponseUtils::createResponse(false, "Invalid or missing token"));  // Return immediately, response is already set
        }
        try {
            std::string result = userService.deleteUser(_user_id);
            return crow::response(ResponseUtils::createResponse(true, "User deleted successfully", result));
        } catch (const std::exception &e) {
            return crow::response(ResponseUtils::createResponse(false, std::string("Error: ") + e.what()));
        } });
}
