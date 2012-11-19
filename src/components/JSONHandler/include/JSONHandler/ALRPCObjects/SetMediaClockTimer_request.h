#ifndef NSAPPLINKRPC_SETMEDIACLOCKTIMER_REQUEST_INCLUDE
#define NSAPPLINKRPC_SETMEDIACLOCKTIMER_REQUEST_INCLUDE


#include "StartTime.h"
#include "UpdateMode.h"
#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Mon Nov 19 10:37:06 2012
  source stamp	Mon Nov 19 10:35:56 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

///  Sets the initial media clock value and automatic update method.

  class SetMediaClockTimer_request : public ALRPCRequest
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
     startTime must be provided for "run"
     startTime will be ignored for "pause" and "resum"
*/
      StartTime* startTime;

/**
     Enumeration to control the media clock.
     In case of pause or resume, the start time value is ignored and shall be left out.  For resume, the time continues with the same value as it was when paused.
*/
      UpdateMode updateMode;
  };

}

#endif
