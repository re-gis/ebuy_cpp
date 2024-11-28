#ifndef CART_SERVICE_HPP
#define CART_SERVICE_HPP

#include <pqxx/pqxx>
#include "models/Cart.hpp"
#include "models/CartItem.hpp"  // Include this if CartItem is in a separate header file
#include "models/Product.hpp"  // Include Product if you are handling product-related operations

class CartService
{
    pqxx::connection &conn;

public:
    CartService(pqxx::connection &conn) : conn(conn) {}

    // Create a new cart for a user
    Cart createCart(int user_id);

    // Get a cart by ID
    Cart getCartById(int cart_id);

    // Get all carts for a user
    std::vector<Cart> getCartsByUserId(int user_id);

    // Add an item to a cart
    std::string addItemToCart(int cart_id, int product_id, int quantity);

    // Remove an item from a cart
    std::string removeItemFromCart(int cart_id, int product_id);

    // Update the quantity of an item in the cart
    std::string updateItemQuantity(int cart_id, int product_id, int new_quantity);

    // Delete a cart by its ID
    std::string deleteCart(int cart_id);

    // Helper function to get CartItems for a given cart ID
    std::vector<CartItem> getCartItems(int cart_id);
};

#endif
