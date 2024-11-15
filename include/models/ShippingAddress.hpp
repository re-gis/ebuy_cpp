#ifndef SHIPPING_ADDRESS_H
#define SHIPPING_ADDRESS_H

#include <string>
#include <ctime>

struct ShippingAddress
{
    int id;
    int user_id;
    std::string country;
    std::string province;
    std::string district;
    std::string sector;
    std::string cell;
    std::string village;
    std::time_t created_at;
    std::time_t updated_at;
};

#endif