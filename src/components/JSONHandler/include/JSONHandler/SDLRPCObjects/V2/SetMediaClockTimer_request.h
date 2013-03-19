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

#ifndef NSSMARTDEVICELINKKRPCV2_SETMEDIACLOCKTIMER_REQUEST_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_SETMEDIACLOCKTIMER_REQUEST_INCLUDE


#include "StartTime.h"
#include "UpdateMode.h"
#include "JSONHandler/SDLRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPCV2
{

///  Sets the initial media clock value and automatic update method.

  class SetMediaClockTimer_request : public NsSmartDeviceLinkRPC::SDLRPCMessage
  {
  public:
  
    SetMediaClockTimer_request(const SetMediaClockTimer_request& c);
    SetMediaClockTimer_request(void);
    
    virtual ~SetMediaClockTimer_request(void);
  
    SetMediaClockTimer_request& operator =(const SetMediaClockTimer_request&);
  
    bool checkIntegrity(void);

    const StartTime* get_startTime(void) const;
    const UpdateMode& get_updateMode(void) const;

    void reset_startTime(void);
    bool set_startTime(const StartTime& startTime_);
    bool set_updateMode(const UpdateMode& updateMode_);

  private:
  
    friend class SetMediaClockTimer_requestMarshaller;


/**
     See StartTime
     startTime must be provided for "COUNTUP" and "COUNTDOWN"
     startTime will be ignored for "PAUSE", "RESUME", and "CLEAR"
*/
      StartTime* startTime;

/**
     Enumeration to control the media clock.
     In case of pause, resume, or clear, the start time value is ignored and shall be left out.  For resume, the time continues with the same value as it was when paused.
*/
      UpdateMode updateMode;
  };

}

#endif
