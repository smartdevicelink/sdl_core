#include "../include/JSONHandler/ALRPCObjects/StartTime_v2.h"
#include "StartTime_v2Marshaller.h"

/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/



using namespace NsAppLinkRPC;

StartTime_v2::StartTime_v2(const StartTime_v2& c)
{
  *this=c;
}


bool StartTime_v2::checkIntegrity(void)
{
  return StartTime_v2Marshaller::checkIntegrity(*this);
}


StartTime_v2::StartTime_v2(void)
{
}



bool StartTime_v2::set_hours(unsigned int hours_)
{
  if(hours_>59)  return false;
  hours=hours_;
  return true;
}

bool StartTime_v2::set_minutes(unsigned int minutes_)
{
  if(minutes_>59)  return false;
  minutes=minutes_;
  return true;
}

bool StartTime_v2::set_seconds(unsigned int seconds_)
{
  if(seconds_>59)  return false;
  seconds=seconds_;
  return true;
}




unsigned int StartTime_v2::get_hours(void) const
{
  return hours;
}


unsigned int StartTime_v2::get_minutes(void) const
{
  return minutes;
}


unsigned int StartTime_v2::get_seconds(void) const
{
  return seconds;
}


