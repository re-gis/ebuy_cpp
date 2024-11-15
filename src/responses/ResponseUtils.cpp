#include "responses/ResponseUtils.hpp"

namespace ResponseUtils
{
    crow::json::wvalue createResponse(bool success, const std::string &message,
                                      const std::string &token
                                      )
    {
        crow::json::wvalue response;
        response["success"] = success;
        response["message"] = message;

        if (!token.empty())
        {
            response["token"] = token;
        }

        // Check if data is valid and not null before adding it to the response
        // if (data.size() > 0)
        // {
        //     // Copy the data content into the response
        //     response["data"] = data;  // Direct assignment instead of move
        // }

        return response;
    }
}
