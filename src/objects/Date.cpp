#include "../headers/Date.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

bool isLeapYear(int year)
{
    return (year % 400 == 0) ||
           (year % 100 != 0 && year % 4 == 0);
}

int daysInMonth(int month, int year)
{
    switch (month)
    {
    case 1:
        return 31;
    case 2:
        return isLeapYear(year) ? 29 : 28;
    case 3:
        return 31;
    case 4:
        return 30;
    case 5:
        return 31;
    case 6:
        return 30;
    case 7:
        return 31;
    case 8:
        return 31;
    case 9:
        return 30;
    case 10:
        return 31;
    case 11:
        return 30;
    case 12:
        return 31;
    default:
        return 0;
    }
}

void Date::nextDay()
{
    day++;

    if (day > daysInMonth(month, year))
    {
        day = 1;
        month++;
        if (month > 12)
        {
            month = 1;
            year++;
        }
    }
}

void Date::addMonths(int months)
{
    int totalMonths = year * 12 + (month - 1) + months;

    year = totalMonths / 12;
    month = totalMonths % 12 + 1;

    // If the current day doesn't exist in the new month,
    // move it to the last valid day.
    int maxDay = daysInMonth(month, year);

    if (day > maxDay)
        day = maxDay;
}

bool Date::isBiggerOrEquals(Date &other)
{
    // cout << "Other Date " << other.toDateString() << "\n";
    // cout << "This Date " << this->toDateString() << "\n";
    if (year != other.year)
    {
        return year >= other.year;
    }
    else if (month != other.month)
    {
        return month >= other.month;
    }
    else
    {
        // cout << (day >= other.day);
        return day >= other.day;
    }
}

bool Date::operator>=(const Date &other) const
{
    if (year != other.year)
        return year >= other.year;

    if (month != other.month)
        return month >= other.month;

    return day >= other.day;
}

string Date::toDateString()
{
    std::ostringstream oss;

    oss << std::setfill('0')
        << std::setw(2) << day << "."
        << std::setw(2) << month << "."
        << std::setw(4) << year;

    return oss.str();
}