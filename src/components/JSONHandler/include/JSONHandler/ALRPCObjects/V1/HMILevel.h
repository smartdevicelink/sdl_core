#ifndef NSAPPLINKRPCV1_HMILEVEL_INCLUDE
#define NSAPPLINKRPCV1_HMILEVEL_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 14:55:10 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV1
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
