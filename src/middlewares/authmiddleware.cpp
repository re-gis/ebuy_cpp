#include "middlewares/authmiddleware.hpp"
#include "database/DatabaseConfig.hpp"
#include "controllers/UserController.hpp"
#include <iostream>
#include <jwt-cpp/jwt.h>
#include <pqxx/pqxx>
#include "services/UserService.hpp"
#include "models/User.hpp"
#include <nlohmann/json.hpp>

// UserService userService;

// Static function to check if the user is logged in based on the authorization header.
bool AuthMiddleware::isLoggedIn(const std::string &authorizationHeader)
{
    // Check if the authorization header is empty or does not start with "Bearer ".
    if (authorizationHeader.empty() || authorizationHeader.find("Bearer ") != 0)
    {
        return false; // Return false if the token is missing or malformed.
    }
    else
    {

        std::string token = authorizationHeader.substr(7); // Remove the "Bearer " prefix

        try
        {
            // The secret key used for token validation
            const std::string secret = "secret";

            // Decode the token and validate it
            auto decoded_token = jwt::decode(token);
            auto verifier = jwt::verify()
                                .allow_algorithm(jwt::algorithm::hs256{secret})
                                .with_issuer("ecommerce_app"); // Set the expected issuer for the token

            // Verify the token with the provided secret key
            nlohmann::json payload = decoded_token.get_payload();

            // Print the payload or some other information
            // std::cout << "Decoded token payload: " << payload.dump() << std::endl;
            verifier.verify(decoded_token);

            return true; // Token is valid
        }
        catch (const std::exception &e)
        {
            std::cerr << "Invalid token or error during validation: " << e.what() << std::endl;
            return false; // Return false if the token is invalid or an error occurs during validation
        }
    }
}

nlohmann::json AuthMiddleware::getLoggedInUser(const std::string &header)
{
    // Check if the authorization header is empty or does not start with "Bearer ".
    if (header.empty() || header.find("Bearer ") != 0)
    {
        return nlohmann::json{{"error", "Authorization header missing or malformed"}};
    }
    else
    {
        // Extract the token by removing the "Bearer " prefix
        std::string token = header.substr(7);

        try
        {
            // The secret key used for token validation
            const std::string secret = "secret";

            // Decode the token
            auto decoded_token = jwt::decode(token);

            // Set up the verifier for token validation
            auto verifier = jwt::verify()
                                .allow_algorithm(jwt::algorithm::hs256{secret})
                                .with_issuer("ecommerce_app"); // Set the expected issuer for the token

            // Verify the token with the provided secret key
            verifier.verify(decoded_token);

            std::cout << decoded_token.get_payload() << std::endl;

            // Extract the payload from the decoded token
            nlohmann::json payload = nlohmann::json::parse(decoded_token.get_payload());

            std::cout << payload["user_id"] << std::endl;
            std::string user_id_str = payload["user_id"].get<std::string>();

            // Convert the string to an integer
            int user_id = std::stoi(user_id_str);
            User user = UserService::getUserById(user_id);
            return user.to_json(); // Assuming the User class has a to_json() method
        }
        catch (const std::exception &e)
        {
            // If there's an error during token validation or fetching user info
            std::cerr << "Invalid token or error during validation: " << e.what() << std::endl;
            return nlohmann::json{{"error", "Invalid token or error during validation"}};
        }
    }
}
