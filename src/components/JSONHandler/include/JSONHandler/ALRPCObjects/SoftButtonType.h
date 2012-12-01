#ifndef NSAPPLINKRPC_SOFTBUTTONTYPE_INCLUDE
#define NSAPPLINKRPC_SOFTBUTTONTYPE_INCLUDE


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
