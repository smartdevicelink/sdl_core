#ifndef NSAPPLINKRPC_TIMERMODE_INCLUDE
#define NSAPPLINKRPC_TIMERMODE_INCLUDE


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
