#ifndef RATING_H
#define RATING_H

#include <string>
#include <ctime>

struct Rating
{
    int id;
    int user_id;
    int product_id;
    int rating : std::string review;
    std::time_t created_at;
};

#endif