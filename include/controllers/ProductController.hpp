#ifndef PRODUCT_CONTROLLER_H
#define PRODUCT_CONTROLLER_H

#include <crow.h>
#include <string>
#include "services/ProductService.hpp"

class ProductController
{
public:
    ProductController(ProductService &service) : productService(service) {}

    void createProductRoutes(crow::SimpleApp &app);
    void getProductByIdRoutes(crow::SimpleApp &app);
    void getAllProductsRoutes(crow::SimpleApp &app);
    void deleteProductRoutes(crow::SimpleApp &app);
    void updateProductRoutes(crow::SimpleApp &app);

private:
    ProductService &productService;
};

#endif
