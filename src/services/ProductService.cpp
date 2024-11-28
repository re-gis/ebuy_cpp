#include "services/ProductService.hpp"
#include <pqxx/pqxx>
#include "models/Product.hpp"
#include <iostream>

Product ProductService::createProduct(const std::string &name, const std::string &category, const std::string &description, double price, int stock)
{
    try
    {
        pqxx::work _txn(conn);
        pqxx::result r = _txn.exec("SELECT * FROM products WHERE name = " + _txn.quote(name) + " AND category = " + _txn.quote(category));
        if (r.size() > 0)
        {
            throw std::runtime_error("Product " + name + " category -> " + category + " already exists");
        }

        std::string _query = "INSERT INTO products (name, description, category, price, stock) VALUES (" + _txn.quote(name) + ", " + _txn.quote(description) + ", " + _txn.quote(category) + ", " + _txn.quote(price) + ", " + _txn.quote(stock) + ")";
        _txn.exec(_query);
        _txn.commit();

        Product _product;
        _product.name = name;
        _product.category = category;
        _product.description = description;
        _product.price = price;
        _product.stock = stock;
        return _product;
    }
    catch (std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        throw;
    }
}

Product ProductService::getProductById(int product_id)
{
    try
    {
        pqxx::work _txn(conn);
        pqxx::result _res = _txn.exec("SELECT id, name, description, category, stock, price FROM products WHERE id = " + _txn.quote(product_id));
        
        if (_res.size() == 0)
        {
            throw std::runtime_error("Product not found!");
        }

        Product _product;
        _product.id = _res[0][0].as<int>();
        
        // Check if the name is NULL and handle it
        _product.name = _res[0][1].is_null() ? "" : _res[0][1].as<std::string>();
        
        // Check if the description is NULL and handle it
        _product.description = _res[0][2].is_null() ? "" : _res[0][2].as<std::string>();
        
        // Check if the category is NULL and handle it
        _product.category = _res[0][3].is_null() ? "" : _res[0][3].as<std::string>();
        
        _product.price = _res[0][5].as<double>();
        _product.stock = _res[0][4].as<int>();

        return _product;
    }
    catch (std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        std::cout << "Error: " << e.what() << std::endl;
        throw;
    }
}


std::vector<Product> ProductService::getAllProducts()
{
    std::vector<Product> _products;
    try
    {
        pqxx::work _txn(conn);
        pqxx::result _res = _txn.exec("SELECT id, name, description, category, price, stock FROM products");
        for (const auto &_row : _res)
        {
            Product _product;
            _product.id = _row[0].as<int>();
            _product.name = _row[1].as<std::string>();
            _product.category = _row[3].as<std::string>();
            _product.description = _row[2].as<std::string>();
            _product.price = _row[4].as<double>();
            _product.stock = _row[5].as<int>();
            _products.push_back(_product);
        }
    }
    catch (std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        throw;
    }

    return _products;
}

std::string ProductService::deleteProduct(int product_id)
{
    try
    {
        pqxx::work _txn(conn);
        std::string _query = "DELETE FROM products WHERE id = " + _txn.quote(product_id);
        _txn.exec(_query);
        _txn.commit();
        std::cout << "Product deleted successfully!" << std::endl;
        return "Product deleted successfully!";
    }
    catch (std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return std::string("Error: ") + e.what();
    }
}

std::string ProductService::updateProduct(int product_id, std::string &name, std::string &description, std::string &category, double price, int stock)
{
    try
    {
        pqxx::work _txn(conn);

        // Check if product exists
        pqxx::result _res = _txn.exec("SELECT id, name, description, category, stock, price FROM products WHERE id = " + _txn.quote(product_id));
        if (_res.size() == 0)
        {
            throw std::runtime_error("Product not found!");
        }

        // Load existing product details
        Product _product;
        _product.name = _res[0][1].as<std::string>();
        _product.category = _res[0][3].as<std::string>();
        _product.description = _res[0][2].as<std::string>();
        _product.price = _res[0][5].as<double>();
        _product.stock = _res[0][4].as<int>();

        // Use existing details if no new values are provided
        if (name.empty())
        {
            name = _product.name;
        }
        if (description.empty())
        {
            description = _product.description;
        }
        if (category.empty())
        {
            category = _product.category;
        }
        if (price == 0)
        {
            price = _product.price;
        }
        if (stock == 0)
        {
            stock = _product.stock;
        }

        // Update the product details in the database
        std::string update_query = "UPDATE products SET name = " + _txn.quote(name) +
                                   ", description = " + _txn.quote(description) +
                                   ", category = " + _txn.quote(category) +
                                   ", price = " + _txn.quote(price) +
                                   ", stock = " + _txn.quote(stock) +
                                   " WHERE id = " + _txn.quote(product_id);

        _txn.exec(update_query);
        _txn.commit();

        return "Product updated successfully!";
    }
    catch (std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return std::string("Error: ") + e.what();
    }
}
