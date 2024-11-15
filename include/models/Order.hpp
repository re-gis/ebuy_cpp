#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <ctime>

struct Order
{
    int id;
    int user_id;
    std::string status;
    double total_amount;
    std::time_t created_at;
    std::time_t updated_at;
};

#endif