#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/SetMediaClockTimer.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


SetMediaClockTimer& SetMediaClockTimer::operator =(const SetMediaClockTimer& c)
{
  if(startTime)  delete startTime;
  startTime= c.startTime ? new NsAppLinkRPC::StartTime(c.startTime[0]) : 0;
  updateMode=c.updateMode;
  appId=c.appId;
  return *this;
}


SetMediaClockTimer::~SetMediaClockTimer(void)
{
  if(startTime)  delete startTime;
}


SetMediaClockTimer::SetMediaClockTimer(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SETMEDIACLOCKTIMER),
  startTime(0)
{
}


SetMediaClockTimer::SetMediaClockTimer(const SetMediaClockTimer& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SETMEDIACLOCKTIMER,c.getId())
{
  *this=c;
}


const NsAppLinkRPC::StartTime* SetMediaClockTimer::get_startTime(void)
{
  return startTime;
}

bool SetMediaClockTimer::set_startTime(const NsAppLinkRPC::StartTime& startTime_)
{
  if(startTime)  delete startTime;
  startTime=new NsAppLinkRPC::StartTime(startTime_);
  return true;
}

void SetMediaClockTimer::reset_startTime(void)
{
  if(startTime)  delete startTime;
  startTime=0;
}

const NsAppLinkRPCV2::UpdateMode& SetMediaClockTimer::get_updateMode(void)
{
  return updateMode;
}

bool SetMediaClockTimer::set_updateMode(const NsAppLinkRPCV2::UpdateMode& updateMode_)
{
  updateMode=updateMode_;
  return true;
}

int SetMediaClockTimer::get_appId(void)
{
  return appId;
}

bool SetMediaClockTimer::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool SetMediaClockTimer::checkIntegrity(void)
{
  return SetMediaClockTimerMarshaller::checkIntegrity(*this);
}
