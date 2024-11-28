#ifndef CART_H
#define CART_H

#include <string>
#include <ctime>
#include "models/CartItem.hpp"

struct Cart
{
    int id;
    int user_id;
    std::vector<CartItem> items;
    std::time_t created_at;
    std::time_t updated_at;
};

#endif