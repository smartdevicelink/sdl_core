#ifndef NSAPPLINKRPCV2_VRCAPABILITIES_INCLUDE
#define NSAPPLINKRPCV2_VRCAPABILITIES_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  Contains information about the VR capabilities.

  class VrCapabilities
  {
  public:
    enum VrCapabilitiesInternal
    {
      INVALID_ENUM=-1,
      VR_TEXT=0
    };
  
    VrCapabilities() : mInternal(INVALID_ENUM)				{}
    VrCapabilities(VrCapabilitiesInternal e) : mInternal(e)		{}
  
    VrCapabilitiesInternal get(void) const	{ return mInternal; }
    void set(VrCapabilitiesInternal e)		{ mInternal=e; }
  
  private:
    VrCapabilitiesInternal mInternal;
    friend class VrCapabilitiesMarshaller;
  };
  
}

#endif
