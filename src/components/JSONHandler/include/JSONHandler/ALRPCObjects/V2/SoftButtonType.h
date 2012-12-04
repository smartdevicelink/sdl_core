#ifndef NSAPPLINKRPCV2_SOFTBUTTONTYPE_INCLUDE
#define NSAPPLINKRPCV2_SOFTBUTTONTYPE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  Contains information about the SoftButton capabilities.

  class SoftButtonType
  {
  public:
    enum SoftButtonTypeInternal
    {
      INVALID_ENUM=-1,
      SBT_TEXT=0,
      SBT_IMAGE=1,
      SBT_BOTH=2
    };
  
    SoftButtonType() : mInternal(INVALID_ENUM)				{}
    SoftButtonType(SoftButtonTypeInternal e) : mInternal(e)		{}
  
    SoftButtonTypeInternal get(void) const	{ return mInternal; }
    void set(SoftButtonTypeInternal e)		{ mInternal=e; }
  
  private:
    SoftButtonTypeInternal mInternal;
    friend class SoftButtonTypeMarshaller;
  };
  
}

#endif
