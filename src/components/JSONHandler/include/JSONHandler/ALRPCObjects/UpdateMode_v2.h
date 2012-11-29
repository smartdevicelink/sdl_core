#ifndef NSAPPLINKRPC_UPDATEMODE_V2_INCLUDE
#define NSAPPLINKRPC_UPDATEMODE_V2_INCLUDE


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
     Describes how the media clock timer should behave on the platform
     Starts the media clock timer counting upwards, as in time elapsed.
     Starts the media clock timer counting downwards, as in time remaining.
     Pauses the media clock timer
     Resume the media clock timer
     Clears the media clock timer (previously done through Show->mediaClock)
*/

  class UpdateMode_v2
  {
  public:
    enum UpdateMode_v2Internal
    {
      INVALID_ENUM=-1,
      COUNTUP=0,
      COUNTDOWN=1,
      PAUSE=2,
      RESUME=3,
      CLEAR=4
    };
  
    UpdateMode_v2() : mInternal(INVALID_ENUM)				{}
    UpdateMode_v2(UpdateMode_v2Internal e) : mInternal(e)		{}
  
    UpdateMode_v2Internal get(void) const	{ return mInternal; }
    void set(UpdateMode_v2Internal e)		{ mInternal=e; }
  
  private:
    UpdateMode_v2Internal mInternal;
    friend class UpdateMode_v2Marshaller;
  };
  
}

#endif
