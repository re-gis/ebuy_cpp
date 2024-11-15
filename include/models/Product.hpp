#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <ctime>

struct Product
{
    int id;
    std::string name;
    std::string description;
    std::string category;
    double price;
    int stock;
    std::time_t created_at;
    std::time_t updated_at;
};

#endif