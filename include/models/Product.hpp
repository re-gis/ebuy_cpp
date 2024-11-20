#ifndef PRODUCT_H
#define PRODUCT_H


#include "crow.h"
#include <string>
#include <nlohmann/json.hpp> 

class Product {
public:
    int id;
    std::string name;
    std::string description;
    std::string category;
    double price;
    int stock;

    nlohmann::json toJson() const {
        nlohmann::json productJson;
        productJson["id"] = id;
        productJson["name"] = name;
        productJson["description"] = description;
        productJson["category"] = category;
        productJson["price"] = price;
        productJson["stock"] = stock;
        return productJson;
    }
};


#endif