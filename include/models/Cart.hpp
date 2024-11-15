#ifndef CART_H
#define CART_H

#include <string>
#include <ctime>

struct Cart
{
    int id;
    int user_id;
    std::time_t created_at;
    std::time_t updated_at;
};

#endif