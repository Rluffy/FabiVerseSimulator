#include "../headers/Date.h"
#include <chrono>
#include <iomanip>
#include <sstream>

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

void Date::addYears(int years)
{
    year += years;

    // Keep 29.02. valid when the target year is not a leap year.
    int maxDay = daysInMonth(month, year);

    if (day > maxDay)
        day = maxDay;
}

int Date::toDayNumber() const
{
    const std::chrono::year_month_day ymd{
        std::chrono::year{year},
        std::chrono::month{static_cast<unsigned>(month)},
        std::chrono::day{static_cast<unsigned>(day)}};

    return static_cast<int>(static_cast<std::chrono::sys_days>(ymd).time_since_epoch().count());
}

Date Date::fromDayNumber(int dayNumber)
{
    const std::chrono::year_month_day ymd{std::chrono::sys_days{std::chrono::days{dayNumber}}};

    return {static_cast<int>(static_cast<unsigned>(ymd.day())),
            static_cast<int>(static_cast<unsigned>(ymd.month())),
            static_cast<int>(ymd.year())};
}

string Date::toDateString() const
{
    std::ostringstream oss;

    oss << std::setfill('0')
        << std::setw(2) << day << "."
        << std::setw(2) << month << "."
        << std::setw(4) << year;

    return oss.str();
}
