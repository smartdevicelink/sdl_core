#ifndef NSAPPLINKRPCV2_SETMEDIACLOCKTIMER_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_SETMEDIACLOCKTIMER_REQUEST_INCLUDE


#include "StartTime.h"
#include "UpdateMode.h"
#include "JSONHandler/ALRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  Sets the initial media clock value and automatic update method.

  class SetMediaClockTimer_request : public NsAppLinkRPC::ALRPCMessage
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
