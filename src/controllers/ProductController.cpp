#include "controllers/ProductController.hpp"
#include "responses/ResponseUtils.hpp"
#include <nlohmann/json.hpp>
#include "middlewares/authmiddleware.hpp"

void ProductController::createProductRoutes(crow::SimpleApp &app)
{
    CROW_ROUTE(app, "/product/create").methods("POST"_method)([this](const crow::request &request)
                                                              {

                std::string auth_header = request.get_header_value("Authorization");

        // Check if the token is missing or invalid
        if (auth_header.empty() || !AuthMiddleware::isLoggedIn(auth_header)) {
            return crow::response(ResponseUtils::createResponse(false, "Invalid or missing token"));  // Return immediately, response is already set
        }
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
            return crow::response(ResponseUtils::createResponse(true, "Product created successfully", "", product.toJson()));
        } catch (const std::exception &e) {
            return crow::response(ResponseUtils::createResponse(false, std::string("Error: ") + e.what()));
        } catch (...) {
            return crow::response(ResponseUtils::createResponse(false, "An unknown error occurred"));
        } });
}

void ProductController::getProductByIdRoutes(crow::SimpleApp &app)
{
    CROW_ROUTE(app, "/product/<int>").methods("GET"_method)([this](int productId)
                                                            {
        try {
            // Get the product
            Product product = productService.getProductById(productId);
            std::cout<<product.toJson()<<std::endl;
            // Prepare and return the response using nlohmann::json
            nlohmann::json response_json = ResponseUtils::createResponse(true, "Product retrieved successfully", "", product.toJson());
            return crow::response(ResponseUtils::createResponse(true, "Product retrieved successfully", "", product.toJson()));
        } catch (const std::exception &e) {
            std::cout << "Error: " << e.what() << std::endl;
            return crow::response(ResponseUtils::createResponse(false, std::string("Error: ") + e.what()));
        } catch (...) {
            return crow::response(ResponseUtils::createResponse(false, "An unknown error occurred"));
        } });
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
            return crow::response(ResponseUtils::createResponse(true, "Products retrieved successfully", "", response_json));
        } catch (const std::exception &e) {
            std::cout << "Error: " << e.what() << std::endl;
            return crow::response(ResponseUtils::createResponse(false, std::string("Error: ") + e.what()));
        } catch (...) {
            return crow::response(ResponseUtils::createResponse(false, "An unknown error occurred"));
        } });
}

void ProductController::deleteProductRoutes(crow::SimpleApp &app)
{
    CROW_ROUTE(app, "/product/delete/<int>").methods("DELETE"_method)([this](const crow::request &req, int productId)
                                                                      {

        std::string auth_header = req.get_header_value("Authorization");
        // Check if the token is missing or invalid
        if (auth_header.empty() || !AuthMiddleware::isLoggedIn(auth_header)) {
            return crow::response(ResponseUtils::createResponse(false, "Invalid or missing token"));  // Return immediately, response is already set
        }
        try {
            // Delete product
            productService.deleteProduct(productId);

            // Prepare and return the response
            nlohmann::json response = ResponseUtils::createResponse(true, "Product deleted successfully");
            return crow::response(response.dump());
        } catch (const std::exception &e) {
            return crow::response(ResponseUtils::createResponse(false, std::string("Error: ") + e.what()));
        } catch (...) {
            return crow::response(ResponseUtils::createResponse(false, "An unknown error occurred"));
        } });
}

void ProductController::updateProductRoutes(crow::SimpleApp &app)
{
    CROW_ROUTE(app, "/product/update/<int>").methods("PUT"_method)([this](const crow::request &request, int productId)
                                                                   {


        std::string auth_header = request.get_header_value("Authorization");

        // Check if the token is missing or invalid
        if (auth_header.empty() || !AuthMiddleware::isLoggedIn(auth_header)) {
            return crow::response(ResponseUtils::createResponse(false, "Invalid or missing token"));  // Return immediately, response is already set
        }
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
            return crow::response(ResponseUtils::createResponse(false, std::string("Error: ") + e.what()));
        } catch (...) {
            return crow::response(ResponseUtils::createResponse(false, "An unknown error occurred"));
        } });
}
