#ifndef DATABASE_CONFIG_HPP
#define DATABASE_CONFIG_HPP

#include <pqxx/pqxx>
#include <memory>
#include <string>

class DatabaseConfig
{
public:
    DatabaseConfig(const std::string &user, const std::string &password, const std::string &db, const std::string &host, const unsigned int port);

    std::shared_ptr<pqxx::connection> getDatabase();

private:
    std::string db_user_;
    std::string db_password_;
    std::string db_name_;
    std::string db_host_;
    unsigned int db_port_;
    std::string conn_string_;
};

#endif
