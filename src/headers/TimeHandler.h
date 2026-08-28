#pragma once

#include "Date.h"
#include "../configs/TimeConfig.h"


class TimeHandler{
public:
 TimeConfig conf;
 Date currentDate;



   TimeHandler(TimeConfig conf);
   void nextDay();
};