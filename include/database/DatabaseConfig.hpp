#ifndef DATABASE_CONFIG_HPP
#define DATABASE_CONFIG_HPP

#include <odb/database.hxx>
#include <odb/pgsql/database.hxx>
#include <memory>
#include <string>

class DatabaseConfig
{
public:
    DatabaseConfig(const std::string &user, const std::string &password, const std::string &db, const std::string &host, const unsigned int port);

    std::shared_ptr<odb::pgsql::database> getDatabase();

private:
    std::string db_user_;
    std::string db_password_;
    std::string db_name_;
    std::string db_host_;
    unsigned int db_port_;
    std::shared_ptr<odb::pgsql::database> db_;
};

#endif