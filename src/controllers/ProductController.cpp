#include "controllers/ProductController.hpp"
#include "responses/ResponseUtils.hpp"
#include <nlohmann/json.hpp>



void ProductController::createProductRoutes(crow::SimpleApp &app)
{
    CROW_ROUTE(app, "/product/create").methods("POST"_method)([this](const crow::request &request)
    {
        try {
            // Parse the incoming JSON body using nlohmann::json
            auto json = nlohmann::json::parse(request.body);

            std::string name = json["name"].get<std::string>();
            std::string description = json["description"].get<std::string>();
            std::string category = json["category"].get<std::string>();
            double price = json["price"].get<double>();
            int stock = json["stock"].get<int>();

            // Create product
            Product product = productService.createProduct(name, category, description, price, stock);

            // Prepare and return the response using nlohmann::json
            nlohmann::json response_json = ResponseUtils::createResponse(true, "Product created successfully", "", product.toJson());
            return crow::response(response_json.dump());
        } catch (const std::exception &e) {
            return crow::response(ResponseUtils::createResponse(false, std::string("Error: ") + e.what()).dump());
        } catch (...) {
            return crow::response(ResponseUtils::createResponse(false, "An unknown error occurred").dump());
        }
    });
}


void ProductController::getProductByIdRoutes(crow::SimpleApp &app)
{
    CROW_ROUTE(app, "/product/<int>").methods("GET"_method)([this](int productId)
    {
        try {
            // Get the product
            Product product = productService.getProductById(productId);

            // Prepare and return the response using nlohmann::json
            nlohmann::json response_json = ResponseUtils::createResponse(true, "Product retrieved successfully", nullptr, product.toJson());
            return crow::response(response_json.dump());
        } catch (const std::exception &e) {
            return crow::response(ResponseUtils::createResponse(false, std::string("Error: ") + e.what()).dump());
        } catch (...) {
            return crow::response(ResponseUtils::createResponse(false, "An unknown error occurred").dump());
        }
    });
}


void ProductController::getAllProductsRoutes(crow::SimpleApp &app)
{
    CROW_ROUTE(app, "/products").methods("GET"_method)([this]()
    {
        try {
            // Fetch all products
            std::vector<Product> products = productService.getAllProducts();

            // Prepare the response JSON array
            nlohmann::json response_json = nlohmann::json::array();
            for (const auto &product : products) {
                response_json.push_back(product.toJson());
            }

            // Return the response
            nlohmann::json response = ResponseUtils::createResponse(true, "Products retrieved successfully", "", response_json);
            return crow::response(response.dump());
        } catch (const std::exception &e) {
            return crow::response(ResponseUtils::createResponse(false, std::string("Error: ") + e.what()).dump());
        } catch (...) {
            return crow::response(ResponseUtils::createResponse(false, "An unknown error occurred").dump());
        }
    });
}

void ProductController::deleteProductRoutes(crow::SimpleApp &app)
{
    CROW_ROUTE(app, "/product/delete/<int>").methods("DELETE"_method)([this](int productId)
    {
        try {
            // Delete product
            productService.deleteProduct(productId);

            // Prepare and return the response
            nlohmann::json response = ResponseUtils::createResponse(true, "Product deleted successfully");
            return crow::response(response.dump());
        } catch (const std::exception &e) {
            return crow::response(ResponseUtils::createResponse(false, std::string("Error: ") + e.what()).dump());
        } catch (...) {
            return crow::response(ResponseUtils::createResponse(false, "An unknown error occurred").dump());
        }
    });
}


void ProductController::updateProductRoutes(crow::SimpleApp &app)
{
    CROW_ROUTE(app, "/product/update/<int>").methods("PUT"_method)([this](const crow::request &request, int productId)
    {
        try {
            // Parse the incoming JSON body using nlohmann::json
            auto json = nlohmann::json::parse(request.body);

            std::string name = json["name"].get<std::string>();
            std::string description = json["description"].get<std::string>();
            std::string category = json["category"].get<std::string>();
            double price = json["price"].get<double>();
            int stock = json["stock"].get<int>();

            // Update the product
            std::string updatedProduct = productService.updateProduct(productId, name, description, category, price, stock);

            // Prepare and return the response using nlohmann::json
            nlohmann::json response_json = ResponseUtils::createResponse(true, "Product updated successfully", nullptr);
            return crow::response(response_json.dump());
        } catch (const std::exception &e) {
            return crow::response(ResponseUtils::createResponse(false, std::string("Error: ") + e.what()).dump());
        } catch (...) {
            return crow::response(ResponseUtils::createResponse(false, "An unknown error occurred").dump());
        }
    });
}
