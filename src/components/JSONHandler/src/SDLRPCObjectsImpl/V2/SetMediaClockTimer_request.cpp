//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/SetMediaClockTimer_request.h"
#include "SetMediaClockTimer_requestMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Marshaller.h"
#include "StartTimeMarshaller.h"
#include "UpdateModeMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;
SetMediaClockTimer_request& SetMediaClockTimer_request::operator =(const SetMediaClockTimer_request& c)
{
  startTime= c.startTime ? new StartTime(c.startTime[0]) : 0;
  updateMode= c.updateMode;

  return *this;
}


SetMediaClockTimer_request::~SetMediaClockTimer_request(void)
{
  if(startTime)
    delete startTime;
}


SetMediaClockTimer_request::SetMediaClockTimer_request(const SetMediaClockTimer_request& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool SetMediaClockTimer_request::checkIntegrity(void)
{
  return SetMediaClockTimer_requestMarshaller::checkIntegrity(*this);
}


SetMediaClockTimer_request::SetMediaClockTimer_request(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION),
      startTime(0)
{
}



bool SetMediaClockTimer_request::set_startTime(const StartTime& startTime_)
{
  if(!StartTimeMarshaller::checkIntegrityConst(startTime_))   return false;
  delete startTime;
  startTime=0;

  startTime=new StartTime(startTime_);
  return true;
}

void SetMediaClockTimer_request::reset_startTime(void)
{
  if(startTime)
    delete startTime;
  startTime=0;
}

bool SetMediaClockTimer_request::set_updateMode(const UpdateMode& updateMode_)
{
  if(!UpdateModeMarshaller::checkIntegrityConst(updateMode_))   return false;
  updateMode=updateMode_;
  return true;
}




const StartTime* SetMediaClockTimer_request::get_startTime(void) const 
{
  return startTime;
}

const UpdateMode& SetMediaClockTimer_request::get_updateMode(void) const 
{
  return updateMode;
}

