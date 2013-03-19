//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#include "../include/JSONHandler/SDLRPCObjects/V1/SetMediaClockTimer_request.h"
#include "SetMediaClockTimer_requestMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V1/Marshaller.h"
#include "StartTimeMarshaller.h"
#include "UpdateModeMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPC;
SetMediaClockTimer_request& SetMediaClockTimer_request::operator =(const SetMediaClockTimer_request& c)
{
  startTime= c.startTime ? new StartTime(c.startTime[0]) : 0;
  updateMode= c.updateMode;

  return *this;}


SetMediaClockTimer_request::~SetMediaClockTimer_request(void)
{
  if(startTime)
    delete startTime;
}


SetMediaClockTimer_request::SetMediaClockTimer_request(const SetMediaClockTimer_request& c)
{
  *this=c;
}


bool SetMediaClockTimer_request::checkIntegrity(void)
{
  return SetMediaClockTimer_requestMarshaller::checkIntegrity(*this);
}


SetMediaClockTimer_request::SetMediaClockTimer_request(void) : SDLRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_SETMEDIACLOCKTIMER_REQUEST),
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

