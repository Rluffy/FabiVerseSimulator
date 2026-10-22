#include "../headers/TimeHandler.h"

TimeHandler::TimeHandler(TimeConfig conf)
    : conf(conf),
      currentDate(conf.startDate),
      currentDay(currentDate.toDayNumber())
{
}

void TimeHandler::nextDay()
{
    currentDate.nextDay();
    currentDay++;
}
