#ifndef NSAPPLINKRPCV2_TIMERMODE_INCLUDE
#define NSAPPLINKRPCV2_TIMERMODE_INCLUDE


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

/**
     Causes the media clock timer to update from 0:00 to a specified time
     Causes the media clock timer to update from a specified time to 0:00
     Indicates to not use the media clock timer
*/

  class TimerMode
  {
  public:
    enum TimerModeInternal
    {
      INVALID_ENUM=-1,
      UP=0,
      DOWN=1,
      NONE=2
    };
  
    TimerMode() : mInternal(INVALID_ENUM)				{}
    TimerMode(TimerModeInternal e) : mInternal(e)		{}
  
    TimerModeInternal get(void) const	{ return mInternal; }
    void set(TimerModeInternal e)		{ mInternal=e; }
  
  private:
    TimerModeInternal mInternal;
    friend class TimerModeMarshaller;
  };
  
}

#endif
