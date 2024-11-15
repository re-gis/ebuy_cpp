#ifndef RESPONSE_UTILS_H
#define RESPONSE_UTILS_H

#include "crow/json.h"
#include <string>

namespace ResponseUtils
{

    crow::json::wvalue createResponse(bool success, const std::string &message,
                                      const std::string &token = "",
                                      const crow::json::wvalue &data = {});
}

#endif
