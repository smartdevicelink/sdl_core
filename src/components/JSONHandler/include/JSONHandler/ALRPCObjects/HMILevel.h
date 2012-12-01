#ifndef NSAPPLINKRPC_HMILEVEL_INCLUDE
#define NSAPPLINKRPC_HMILEVEL_INCLUDE


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
