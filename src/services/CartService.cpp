#include "CartService.hpp"

Cart CartService::createCart(int user_id)
{
    pqxx::work txn(conn);

    // Create a new cart for the user
    std::string query = "INSERT INTO carts (user_id, created_at, updated_at) "
                        "VALUES (" +
                        std::to_string(user_id) + ", NOW(), NOW()) RETURNING id;";

    pqxx::result res = txn.exec(query);
    int cart_id = res[0][0].as<int>(); // Get the newly created cart's ID

    txn.commit();

    // Return a new Cart object
    return Cart{cart_id, user_id, std::time(nullptr), std::time(nullptr)};
}

Cart CartService::getCartById(int cart_id)
{
    pqxx::work txn(conn);

    std::string query = "SELECT * FROM carts WHERE id = " + std::to_string(cart_id);
    pqxx::result res = txn.exec(query);

    if (res.empty())
    {
        throw std::runtime_error("Cart not found");
    }

    // Assuming you have a Cart object constructor to handle this
    Cart cart = {res[0][0].as<int>(), res[0][1].as<int>(), res[0][2].as<std::time_t>(), res[0][3].as<std::time_t>()};

    txn.commit();
    return cart;
}

std::vector<Cart> CartService::getCartsByUserId(int user_id)
{
    pqxx::work txn(conn);

    std::string query = "SELECT * FROM carts WHERE user_id = " + std::to_string(user_id);
    pqxx::result res = txn.exec(query);

    std::vector<Cart> carts;
    for (const auto &row : res)
    {
        Cart cart = {row[0].as<int>(), row[1].as<int>(), row[2].as<std::time_t>(), row[3].as<std::time_t>()};
        carts.push_back(cart);
    }

    txn.commit();
    return carts;
}

std::string CartService::addItemToCart(int cart_id, int product_id, int quantity)
{
    pqxx::work txn(conn);

    // First, check if the product is already in the cart
    std::string check_query = "SELECT * FROM cart_items WHERE cart_id = " + std::to_string(cart_id) + " AND product_id = " + std::to_string(product_id);
    pqxx::result res = txn.exec(check_query);

    if (res.empty())
    {
        // Product is not in the cart, add it
        std::string insert_query = "INSERT INTO cart_items (cart_id, product_id, quantity) "
                                   "VALUES (" +
                                   std::to_string(cart_id) + ", " + std::to_string(product_id) + ", " + std::to_string(quantity) + ");";
        txn.exec(insert_query);
    }
    else
    {
        // Product is already in the cart, update the quantity
        int current_quantity = res[0][2].as<int>();
        int new_quantity = current_quantity + quantity;

        std::string update_query = "UPDATE cart_items SET quantity = " + std::to_string(new_quantity) +
                                   " WHERE cart_id = " + std::to_string(cart_id) + " AND product_id = " + std::to_string(product_id);
        txn.exec(update_query);
    }

    txn.commit();
    return "Item added to cart successfully";
}

std::string CartService::removeItemFromCart(int cart_id, int product_id)
{
    pqxx::work txn(conn);

    std::string query = "DELETE FROM cart_items WHERE cart_id = " + std::to_string(cart_id) + " AND product_id = " + std::to_string(product_id);
    txn.exec(query);

    txn.commit();
    return "Item removed from cart successfully";
}

std::string CartService::updateItemQuantity(int cart_id, int product_id, int new_quantity)
{
    pqxx::work txn(conn);

    std::string query = "UPDATE cart_items SET quantity = " + std::to_string(new_quantity) +
                        " WHERE cart_id = " + std::to_string(cart_id) + " AND product_id = " + std::to_string(product_id);
    txn.exec(query);

    txn.commit();
    return "Item quantity updated successfully";
}

std::string CartService::deleteCart(int cart_id)
{
    pqxx::work txn(conn);

    // Delete all cart items
    std::string delete_items_query = "DELETE FROM cart_items WHERE cart_id = " + std::to_string(cart_id);
    txn.exec(delete_items_query);

    // Delete the cart itself
    std::string delete_cart_query = "DELETE FROM carts WHERE id = " + std::to_string(cart_id);
    txn.exec(delete_cart_query);

    txn.commit();
    return "Cart deleted successfully";
}

std::vector<CartItem> CartService::getCartItems(int cart_id)
{
    pqxx::work txn(conn);

    std::string query = "SELECT * FROM cart_items WHERE cart_id = " + std::to_string(cart_id);
    pqxx::result res = txn.exec(query);

    std::vector<CartItem> cart_items;
    for (const auto &row : res)
    {
        CartItem item = {row[1].as<int>(), row[2].as<int>()}; // Assuming cart_item columns are cart_id, product_id, quantity
        cart_items.push_back(item);
    }

    txn.commit();
    return cart_items;
}
