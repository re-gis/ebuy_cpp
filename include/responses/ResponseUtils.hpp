#ifndef RESPONSE_UTILS_H
#define RESPONSE_UTILS_H

#include <string>
#include <optional>
#include <nlohmann/json.hpp>

namespace ResponseUtils
{
    // Struct for API response
    struct ApiResponse
    {
        bool success;
        std::string message;
        std::optional<std::string> token;
        std::optional<nlohmann::json> data;

        // Function to convert ApiResponse to nlohmann::json
        nlohmann::json to_json() const
        {
            nlohmann::json response;
            response["success"] = success;
            response["message"] = message;

            if (token)
                response["token"] = *token;

            if (data)
                response["data"] = *data;

            return response;
        }
    };

    // Function to create a response in JSON format (as string)
    std::string createResponse(bool success, const std::string &message,
                               const std::string &token = "",
                               const nlohmann::json &data = {});
}

#endif // RESPONSE_UTILS_H
