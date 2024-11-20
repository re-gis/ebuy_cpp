#include "services/UserService.hpp"
#include <iostream>
#include <vector>
#include <crypt.h>
#include <pqxx/pqxx>
#include <jwt-cpp/jwt.h>

std::string UserService::hashPassword(const std::string &password)
{
    const char *salt = "$6$123456$";
    char *hashedPassword = crypt(password.c_str(), salt);
    return std::string(hashedPassword);
}

std::string UserService::hashAnyString(const std::string &_mystring)
{
    const char *salt = "$6$123456$";
    char *hashedString = crypt(_mystring.c_str(), salt);
    return std::string(hashedString);
}

std::string UserService::generateToken(int user_id, const std::string &email, const std::string &role, std::string &username)
{
    const std::string secret = "secret";
    auto token = jwt::create()
                     .set_issuer("ecommerce_app")
                     .set_subject("user_login")
                     .set_payload_claim("user_id", jwt::claim(hashAnyString(std::to_string(user_id))))
                     .set_payload_claim("username", jwt::claim(hashAnyString(username)))
                     .set_payload_claim("email", jwt::claim(hashAnyString(email)))
                     .set_payload_claim("role", jwt::claim(hashAnyString(role)))
                     .set_expires_at(std::chrono::system_clock::now() + std::chrono::hours(24))
                     .sign(jwt::algorithm::hs256{secret});
    return token;
}

std::string UserService::registerUser(const std::string &username, const std::string &email, const std::string &password, const std::string &role)
{
    std::string hashedPassword = hashPassword(password);
    try
    {
        pqxx::work txn(conn);
        // check if the user already exists
        std::string _select_query = "SELECT * FROM users WHERE email = " + txn.quote(email);
        pqxx::result r = txn.exec(_select_query);
        if (r.size() > 0)
        {
            return std::string("Email ") + email + std::string(" already taken.");
        }

        std::string query = "INSERT INTO users (username, email,password, role) VALUES (" + txn.quote(username) + ", " + txn.quote(email) + ", " + txn.quote(hashedPassword) + ", " + txn.quote(role) + ")";
        txn.exec(query);
        txn.commit();
        std::cout << "User registered successfully, login to continue..." << std::endl;
        return "User registered successfully, login to continue...";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return std::string("Error: ") + e.what();
    }
}

std::string UserService::login(const std::string &email, const std::string &password)
{
    try
    {
        pqxx::work txn(conn);
        pqxx::result r = txn.exec("SELECT id, username, email, role, password FROM users WHERE email = " + txn.quote(email));
        if (r.size() == 0)
        {
            std::cout << "User not found!" << std::endl;
            return "Invalid email or password!";
        }

        int _user_id = r[0][0].as<int>();
        std::string _username = r[0][0].as<std::string>();
        std::string _email = r[0][2].as<std::string>();
        std::string _role = r[0][3].as<std::string>();
        std::string _storeHashedPass = r[0][4].as<std::string>();
        const char *salt = _storeHashedPass.substr(0, 12).c_str();
        char *_hashedInputPass = crypt(password.c_str(), salt);

        if (_hashedInputPass == _storeHashedPass)
        {
            std::string token = generateToken(_user_id, _email, _role, _username);
            return token;
        }
        else
        {
            std::cout << "Invalid password!" << std::endl;
            return "Invalid email or password!";
        }
    }
    catch (std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        return std::string("Error") + e.what();
    }
}

std::string UserService::updateUser(int user_id, std::string &username, std::string &email, std::string &password)
{
    try
    {
        pqxx::work txn(conn);
        pqxx::result r = txn.exec("SELECT username, email, password FROM users WHERE user_id = " + user_id);
        if (r.size() == 0)
        {
            std::cout << "User not found!" << std::endl;
            return "User not found!";
        }

        std::string _username = r[0][0].as<std::string>();
        std::string _email = r[0][1].as<std::string>();
        std::string _password = r[0][2].as<std::string>();

        if (username.empty())
        {
            username = _username;
        }

        if (email.empty())
        {
            email = _email;
        }

        if (password.empty())
        {
            password = _password;
        }
        else
        {
            password = hashPassword(password);
        }

        std::string query = "UPDATE users SET username = " + txn.quote(username) + ", email = " + txn.quote(email) + ", password = " + txn.quote(password);
        txn.exec(query);
        txn.commit();
        std::cout << "User profile updated successfully!" << std::endl;
        return "User profile updated successfully!";
    }
    catch (std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        return std::string("Error") + e.what();
    }
}

User UserService::getUserById(int user_id)
{
    try
    {
        pqxx::work txn(conn);
        pqxx::result r = txn.exec("SELECT id, username, email, role FROM users WHERE id = " + txn.quote(user_id));
        if (r.size() > 0)
        {
            User user;
            user.id = r[0][0].as<int>();
            user.username = r[0][1].as<std::string>();
            user.email = r[0][2].as<std::string>();
            user.role = r[0][3].as<std::string>();
            return user;
        }
        else
        {
            throw std::runtime_error("User not found!");
        }
    }
    catch (std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        throw;
    }
}

std::vector<User> UserService::getAllUsersByRole(const std::string &role)
{
    std::vector<User> users;
    try
    {
        pqxx::work txn(conn);
        pqxx::result r = txn.exec("SELECT id, username, email, role FROM users WHERE role = " + txn.quote(role));

        for (const auto &row : r)
        {
            User user;
            user.id = row[0].as<int>();
            user.username = row[1].as<std::string>();
            user.email = row[2].as<std::string>();
            user.role = row[3].as<std::string>();
            users.push_back(user);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return users;
}

std::string UserService::deleteUser(int userId)
{
    try
    {
        pqxx::work txn(conn);
        std::string query = "DELETE FROM users WHERE id = " + txn.quote(userId);
        txn.exec(query);
        txn.commit();
        std::cout << "User deleted successfully!" << std::endl;
        return "User deleted successfully!";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return std::string("Error: ") + e.what();
    }
}

User UserService::getLoggedInUser(int userId)
{
    try
    {
        pqxx::work txn(conn);
        pqxx::result r = txn.exec("SELECT id, username, email, role FROM users WHERE id = " + txn.quote(userId));

        if (r.size() > 0)
        {
            User user;
            user.id = r[0][0].as<int>();
            user.username = r[0][1].as<std::string>();
            user.email = r[0][2].as<std::string>();
            user.role = r[0][3].as<std::string>();
            return user;
        }
        else
        {
            throw std::runtime_error("Logged in user not found!");
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }
}