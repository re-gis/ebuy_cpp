#include "database/DatabaseConfig.hpp"

DatabaseConfig::DatabaseConfig(const std::string &user, const std::string &password,
                               const std::string &db, const std::string &host,
                               const unsigned int port)
    : db_user_(user), db_password_(password), db_name_(db), db_host_(host), db_port_(port)
{
    conn_string_ = "dbname=" + db_name_ +
                   " user=" + db_user_ +
                   " password=" + db_password_ +
                   " host=" + db_host_ +
                   " port=" + std::to_string(db_port_);
}

std::shared_ptr<pqxx::connection> DatabaseConfig::getDatabase()
{
    try
    {
        auto connection = std::make_shared<pqxx::connection>(conn_string_);
        if (!connection->is_open())
        {
            throw std::runtime_error("Failed to connect to the database.");
        }
        return connection;
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error(std::string("Database connection error: ") + e.what());
    }
}
