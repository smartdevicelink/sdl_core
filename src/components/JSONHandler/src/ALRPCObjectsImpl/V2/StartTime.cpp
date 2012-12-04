#include "../include/JSONHandler/ALRPCObjects/V2/StartTime.h"
#include "StartTimeMarshaller.h"

/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/



using namespace NsAppLinkRPCV2;

StartTime::StartTime(const StartTime& c)
{
  *this=c;
}


bool StartTime::checkIntegrity(void)
{
  return StartTimeMarshaller::checkIntegrity(*this);
}


StartTime::StartTime(void)
{
}



bool StartTime::set_hours(unsigned int hours_)
{
  if(hours_>59)  return false;
  hours=hours_;
  return true;
}

bool StartTime::set_minutes(unsigned int minutes_)
{
  if(minutes_>59)  return false;
  minutes=minutes_;
  return true;
}

bool StartTime::set_seconds(unsigned int seconds_)
{
  if(seconds_>59)  return false;
  seconds=seconds_;
  return true;
}




unsigned int StartTime::get_hours(void) const
{
  return hours;
}


unsigned int StartTime::get_minutes(void) const
{
  return minutes;
}


unsigned int StartTime::get_seconds(void) const
{
  return seconds;
}


