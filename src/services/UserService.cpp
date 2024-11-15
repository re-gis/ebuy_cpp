#include "services/UserService.hpp"
#include <iostream>
#include <crypt.h>
#include <odb/transaction.hxx>
#include "../models/User.hpp"

// Hash password using crypt
std::string UserService::hashPassword(const std::string &password)
{
    const char *salt = "$6$123456$";
    char *hashedPassword = crypt(password.c_str(), salt);
    
    // Handle if crypt returns NULL
    if (hashedPassword == NULL)
    {
        std::cerr << "Error hashing password!" << std::endl;
        return "";  // Or handle it with an exception or error message
    }

    return std::string(hashedPassword);
}

// Hash any string
std::string UserService::hashAnyString(const std::string &_mystring)
{
    const char *salt = "$6$123456$";
    char *hashedString = crypt(_mystring.c_str(), salt);
    
    // Handle if crypt returns NULL
    if (hashedString == NULL)
    {
        std::cerr << "Error hashing string!" << std::endl;
        return "";  // Or handle it with an exception or error message
    }

    return std::string(hashedString);
}

// Register user
std::string UserService::registerUser(const std::string &username, const std::string &email, const std::string &password, const std::string &role)
{
    try
    {
        odb::transaction t(db_->begin());  // Ensure db_ is initialized and open
        std::shared_ptr<User> newUser = std::make_shared<User>(username, email, hashPassword(password), role);
        db_->persist(newUser);  // Persist the user in the database
        t.commit();  // Commit the transaction
        return "User registered successfully!";
    }
    catch (const odb::exception &e)
    {
        std::cerr << "Database error: " << e.what() << std::endl;
        return "Registration failed!";
    }
}

// Get user by ID
std::shared_ptr<User> UserService::getUserById(int user_id)
{
    try
    {
        odb::transaction t(db_->begin());  // Begin transaction
        std::shared_ptr<User> user = db_->load<User>(user_id);  // Load user by ID
        t.commit();  // Commit the transaction
        
        if (user == nullptr)
        {
            std::cerr << "User not found!" << std::endl;
            return nullptr;  // Return nullptr if user not found
        }
        
        return user;  // Return user
    }
    catch (const odb::exception &e)
    {
        std::cerr << "Error fetching user: " << e.what() << std::endl;
        throw std::runtime_error("Error fetching user.");
    }
}

// Delete user
std::string UserService::deleteUser(int userId)
{
    try
    {
        odb::transaction t(db_->begin());  // Begin transaction
        db_->erase<User>(userId);  // Erase user from database
        t.commit();  // Commit the transaction
        return "User deleted successfully!";
    }
    catch (const odb::exception &e)
    {
        std::cerr << "Error deleting user: " << e.what() << std::endl;
        return "Error deleting user!";
    }
}
