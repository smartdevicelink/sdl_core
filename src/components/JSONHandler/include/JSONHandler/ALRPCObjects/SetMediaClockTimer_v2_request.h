#ifndef NSAPPLINKRPC_SETMEDIACLOCKTIMER_V2_REQUEST_INCLUDE
#define NSAPPLINKRPC_SETMEDIACLOCKTIMER_V2_REQUEST_INCLUDE


#include "StartTime_v2.h"
#include "UpdateMode_v2.h"
#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

///  Sets the initial media clock value and automatic update method.

  class SetMediaClockTimer_v2_request : public ALRPCRequest
  {
  public:
  
    SetMediaClockTimer_v2_request(const SetMediaClockTimer_v2_request& c);
    SetMediaClockTimer_v2_request(void);
    
    virtual ~SetMediaClockTimer_v2_request(void);
  
    SetMediaClockTimer_v2_request& operator =(const SetMediaClockTimer_v2_request&);
  
    bool checkIntegrity(void);

    const StartTime_v2* get_startTime(void) const;
    const UpdateMode_v2* get_updateMode(void) const;

    void reset_startTime(void);
    bool set_startTime(const StartTime_v2& startTime_);
    void reset_updateMode(void);
    bool set_updateMode(const UpdateMode_v2& updateMode_);

  private:
  
    friend class SetMediaClockTimer_v2_requestMarshaller;


/**
     See StartTime
     startTime must be provided for "COUNTUP" and "COUNTDOWN"
     startTime will be ignored for "PAUSE", "RESUME", and "CLEAR"
*/
      StartTime_v2* startTime;

/**
     Enumeration to control the media clock.
     In case of pause, resume, or clear, the start time value is ignored and shall be left out.  For resume, the time continues with the same value as it was when paused.
*/
      UpdateMode_v2* updateMode;
  };

}

#endif
