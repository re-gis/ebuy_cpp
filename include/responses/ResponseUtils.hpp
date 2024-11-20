#ifndef RESPONSE_UTILS_H
#define RESPONSE_UTILS_H

#include "crow/json.h"
#include <string>
#include <nlohmann/json.hpp> 

namespace ResponseUtils
{

    ApiResponse createResponse(bool success, const std::string &message,
                           const std::string &token = "",
                           const nlohmann::json &data = {});
}

#endif
