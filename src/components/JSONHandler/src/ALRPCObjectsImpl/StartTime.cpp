#include "../include/JSONHandler/ALRPCObjects/StartTime.h"
#include "StartTimeMarshaller.h"

/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Mon Nov 19 10:37:06 2012
  source stamp	Mon Nov 19 10:35:56 2012
  author	robok0der
*/



using namespace NsAppLinkRPC;

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


