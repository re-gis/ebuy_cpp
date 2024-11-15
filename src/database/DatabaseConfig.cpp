#include "database/DatabaseConfig.hpp"
#include <odb/pgsql/database.hxx>
#include <iostream>

DatabaseConfig::DatabaseConfig(const std::string &user, const std::string &password, const std::string &db, const std::string &host, const unsigned int port)
    : db_user_(user), db_password_(password), db_host_(host), db_port_(port), db_name_(db)
{
    try
    {
        db_ = std::make_shared<odb::pgsql::database>(db_user_, db_password_, db_name_, db_host_, std::to_string(db_port_));
        std::cout << "Database connected successfully..." << std::endl;
    }
    catch (const odb::exception &e)
    {
        std::cerr << "Database connection error: " << e.what() << std::endl;
    }
}

std::shared_ptr<odb::pgsql::database> DatabaseConfig::getDatabase()
{
    return db_;
}