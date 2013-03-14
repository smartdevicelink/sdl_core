#ifndef NSAPPLINKRPC_HMILEVEL_INCLUDE
#define NSAPPLINKRPC_HMILEVEL_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	robok0der
*/

namespace NsSmartDeviceLinkRPC
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
