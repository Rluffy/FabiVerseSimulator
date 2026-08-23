#pragma once

#include <string>

struct Date
{
    int day;
    int month;
    int year;

    void nextDay();
    void addMonths(int months);
    std::string toDateString();

    bool isBiggerOrEquals(Date &other);
    bool operator>=(const Date &other) const;


};
