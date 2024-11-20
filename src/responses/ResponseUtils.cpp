#include "crow.h" // Include the Crow header file
#include <string>
#include <optional>
#include <nlohmann/json.hpp> // Include nlohmann::json for JSON handling

namespace ResponseUtils
{
    // Define the ApiResponse struct with nlohmann::json for 'data'
    struct ApiResponse
    {
        bool success;
        std::string message;
        std::optional<std::string> token;   // Optional token
        std::optional<nlohmann::json> data; // Optional nlohmann::json data

        // Convert the ApiResponse to JSON using nlohmann::json
        nlohmann::json to_json() const
        {
            nlohmann::json response;
            response["success"] = success;
            response["message"] = message;

            // Include token if it's provided
            if (token)
            {
                response["token"] = *token;
            }

            // Include data if it's provided
            if (data)
            {
                response["data"] = *data;
            }

            return response;
        }
    };

    // Function to create the response using ApiResponse struct
    ApiResponse createResponse(bool success, const std::string &message,
                               const std::string &token = "",
                               const nlohmann::json &data = {})
    {
        ApiResponse response;
        response.success = success;
        response.message = message;

        // Set token if it's not empty
        if (!token.empty())
        {
            response.token = token;
        }

        // Set data if it's not empty
        if (!data.is_null())
        {
            response.data = data;
        }

        return response;
    }
}
