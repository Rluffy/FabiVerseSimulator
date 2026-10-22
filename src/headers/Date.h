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
    std::string toDateString() const;

    // Days since 1970-01-01. The simulation compares dates as these plain integers.
    int toDayNumber() const;
    static Date fromDayNumber(int dayNumber);
};
