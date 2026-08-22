#pragma once

#include "Date.h"



class TimeHandler{
public:
   Date currentDate;
   int pregnancyTimeMonths;

   TimeHandler(Date currentDate, int pregnancyTimeMonths);
   void nextDay();
};