#include "../../include/JSONHandler/ALRPCObjects/StartTime.h"
#include "StartTimeMarshaller.h"

/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/



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


