//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_HMILEVEL_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_HMILEVEL_INCLUDE


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

///  Enumeraction that describes current levels of HMI.

  class HMILevel
  {
  public:
    enum HMILevelInternal
    {
      INVALID_ENUM=-1,
      HMI_FULL=0,
      HMI_LIMITED=1,
      HMI_BACKGROUND=2,
      HMI_NONE=3
    };
  
    HMILevel() : mInternal(INVALID_ENUM)				{}
    HMILevel(HMILevelInternal e) : mInternal(e)		{}
  
    HMILevelInternal get(void) const	{ return mInternal; }
    void set(HMILevelInternal e)		{ mInternal=e; }
  
  private:
    HMILevelInternal mInternal;
    friend class HMILevelMarshaller;
  };
  
}

#endif
