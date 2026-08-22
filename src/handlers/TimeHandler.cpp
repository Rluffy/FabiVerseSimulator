

#include "../headers/TimeHandler.h"

using namespace std;


TimeHandler::TimeHandler(Date currentDate, int pregnancyTimeMonths)
:currentDate(currentDate),
pregnancyTimeMonths(pregnancyTimeMonths)
{


}

void TimeHandler::nextDay(){
    currentDate.nextDay();
}
