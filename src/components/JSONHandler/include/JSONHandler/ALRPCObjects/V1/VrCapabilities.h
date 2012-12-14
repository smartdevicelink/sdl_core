#ifndef NSAPPLINKRPC_VRCAPABILITIES_INCLUDE
#define NSAPPLINKRPC_VRCAPABILITIES_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Dec 14 08:11:37 2012
  source stamp	Fri Dec 14 08:11:34 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

///  Contains information about the VR capabilities.

  class VrCapabilities
  {
  public:
    enum VrCapabilitiesInternal
    {
      INVALID_ENUM=-1,
      Text=0
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
