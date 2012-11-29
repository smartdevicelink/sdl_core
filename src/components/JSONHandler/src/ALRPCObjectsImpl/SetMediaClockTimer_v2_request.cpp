#include "../include/JSONHandler/ALRPCObjects/SetMediaClockTimer_v2_request.h"
#include "SetMediaClockTimer_v2_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "StartTime_v2Marshaller.h"
#include "UpdateMode_v2Marshaller.h"

#define PROTOCOL_VERSION	42


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;
SetMediaClockTimer_v2_request& SetMediaClockTimer_v2_request::operator =(const SetMediaClockTimer_v2_request& c)
{
  startTime= c.startTime ? new StartTime_v2(c.startTime[0]) : 0;
  updateMode= c.updateMode ? new UpdateMode_v2(c.updateMode[0]) : 0;

  return *this;}


SetMediaClockTimer_v2_request::~SetMediaClockTimer_v2_request(void)
{
  if(startTime)
    delete startTime;
  if(updateMode)
    delete updateMode;
}


SetMediaClockTimer_v2_request::SetMediaClockTimer_v2_request(const SetMediaClockTimer_v2_request& c)
{
  *this=c;
}


bool SetMediaClockTimer_v2_request::checkIntegrity(void)
{
  return SetMediaClockTimer_v2_requestMarshaller::checkIntegrity(*this);
}


SetMediaClockTimer_v2_request::SetMediaClockTimer_v2_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_SETMEDIACLOCKTIMER_V2_REQUEST),
      startTime(0),
    updateMode(0)
{
}



bool SetMediaClockTimer_v2_request::set_startTime(const StartTime_v2& startTime_)
{
  if(!StartTime_v2Marshaller::checkIntegrityConst(startTime_))   return false;
  delete startTime;
  startTime=0;

  startTime=new StartTime_v2(startTime_);
  return true;
}

void SetMediaClockTimer_v2_request::reset_startTime(void)
{
  if(startTime)
    delete startTime;
  startTime=0;
}

bool SetMediaClockTimer_v2_request::set_updateMode(const UpdateMode_v2& updateMode_)
{
  if(!UpdateMode_v2Marshaller::checkIntegrityConst(updateMode_))   return false;
  delete updateMode;
  updateMode=0;

  updateMode=new UpdateMode_v2(updateMode_);
  return true;
}

void SetMediaClockTimer_v2_request::reset_updateMode(void)
{
  if(updateMode)
    delete updateMode;
  updateMode=0;
}




const StartTime_v2* SetMediaClockTimer_v2_request::get_startTime(void) const 
{
  return startTime;
}

const UpdateMode_v2* SetMediaClockTimer_v2_request::get_updateMode(void) const 
{
  return updateMode;
}

