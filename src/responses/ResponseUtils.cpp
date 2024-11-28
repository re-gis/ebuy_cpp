#include "responses/ResponseUtils.hpp"

namespace ResponseUtils
{
    // Create a JSON response and return as a string
    std::string createResponse(bool success, const std::string &message,
                               const std::string &token,
                               const nlohmann::json &data)
    {
        ApiResponse response;
        response.success = success;
        response.message = message;

        if (!token.empty())
        {
            response.token = token;
        }

        if (!data.is_null())
        {
            response.data = data;
        }

        return response.to_json().dump();
    }
}
