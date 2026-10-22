#pragma once

#include "Date.h"
#include "../configs/TimeConfig.h"

class TimeHandler
{
public:
   explicit TimeHandler(TimeConfig conf);
   void nextDay();

   TimeConfig conf;
   Date currentDate;
   // Days since 1970-01-01, always equal to currentDate.toDayNumber()
   int currentDay;
};
