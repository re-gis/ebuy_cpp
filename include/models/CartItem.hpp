#ifndef CART_ITEM_H
#define CART_ITEM_H

#include <string>
#include <ctime>
#include <crow.h>
#include <nlohmann/json.hpp>

class CartItem
{
public:
    int product_id;
    int quantity;

    CartItem(int product_id, int quantity) : product_id(product_id), quantity(quantity) {}
};

#endif
