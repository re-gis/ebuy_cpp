#ifndef PRODUCT_SERVICE_HPP
#define PRODUCT_SERVICE_HPP

#include <pqxx/pqxx>
#include "models/Product.hpp"

class ProductService
{
    pqxx::connection &conn;

public:
    ProductService(pqxx::connection &conn) : conn(conn) {}

    Product createProduct(const std::string &name, const std::string &category, const std::string &description, double price, int stock);
    Product getProductById(int product_id);
    std::vector<Product> getAllProducts();
    std::string updateProduct(int product_id, std::string &name, std::string &description, std::string &category, double price, int stock);
    std::string deleteProduct(int product_id);
};

#endif