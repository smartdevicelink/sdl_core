#ifndef NSAPPLINKRPC_VRCAPABILITIES_V2_INCLUDE
#define NSAPPLINKRPC_VRCAPABILITIES_V2_INCLUDE


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

///  Contains information about the VR capabilities.

  class VrCapabilities_v2
  {
  public:
    enum VrCapabilities_v2Internal
    {
      INVALID_ENUM=-1,
      VR_TEXT=0
    };
  
    VrCapabilities_v2() : mInternal(INVALID_ENUM)				{}
    VrCapabilities_v2(VrCapabilities_v2Internal e) : mInternal(e)		{}
  
    VrCapabilities_v2Internal get(void) const	{ return mInternal; }
    void set(VrCapabilities_v2Internal e)		{ mInternal=e; }
  
  private:
    VrCapabilities_v2Internal mInternal;
    friend class VrCapabilities_v2Marshaller;
  };
  
}

#endif
