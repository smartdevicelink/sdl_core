//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPC_UPDATEMODE_INCLUDE
#define NSSMARTDEVICELINKKRPC_UPDATEMODE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPC
{

/**
     Describes how the media clock timer should behave on the platform
     Starts the media clock timer counting upwards, as in time elapsed.
     Starts the media clock timer counting downwards, as in time remaining.
     Pauses the media clock timer
     Resume the media clock timer
*/

  class UpdateMode
  {
  public:
    enum UpdateModeInternal
    {
      INVALID_ENUM=-1,
      COUNTUP=0,
      COUNTDOWN=1,
      PAUSE=2,
      RESUME=3
    };
  
    UpdateMode() : mInternal(INVALID_ENUM)				{}
    UpdateMode(UpdateModeInternal e) : mInternal(e)		{}
  
    UpdateModeInternal get(void) const	{ return mInternal; }
    void set(UpdateModeInternal e)		{ mInternal=e; }
  
  private:
    UpdateModeInternal mInternal;
    friend class UpdateModeMarshaller;
  };
  
}

#endif
