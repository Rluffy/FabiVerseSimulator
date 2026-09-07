#pragma once

#include <string>

struct Date
{
    int day;
    int month;
    int year;

    void nextDay();
    void addMonths(int months);
    void addYears(int years);
    void addDays(int days);
    std::string toDateString() const;

    bool isBiggerOrEquals(Date &other);
    bool operator>=(const Date &other) const;
};
