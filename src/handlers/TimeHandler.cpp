

#include "../headers/TimeHandler.h"

using namespace std;


TimeHandler::TimeHandler(TimeConfig conf)
:conf(conf),
currentDate(conf.startDate)
{


}

void TimeHandler::nextDay(){
    currentDate.nextDay();
}
