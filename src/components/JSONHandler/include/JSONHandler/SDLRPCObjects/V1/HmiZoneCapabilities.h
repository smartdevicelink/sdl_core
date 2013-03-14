#ifndef NSAPPLINKRPC_HMIZONECAPABILITIES_INCLUDE
#define NSAPPLINKRPC_HMIZONECAPABILITIES_INCLUDE


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

/**
     Contains information about the HMI zone capabilities.
     For future use.
*/

  class HmiZoneCapabilities
  {
  public:
    enum HmiZoneCapabilitiesInternal
    {
      INVALID_ENUM=-1,
      FRONT=0,
      BACK=1
    };
  
    HmiZoneCapabilities() : mInternal(INVALID_ENUM)				{}
    HmiZoneCapabilities(HmiZoneCapabilitiesInternal e) : mInternal(e)		{}
  
    HmiZoneCapabilitiesInternal get(void) const	{ return mInternal; }
    void set(HmiZoneCapabilitiesInternal e)		{ mInternal=e; }
  
  private:
    HmiZoneCapabilitiesInternal mInternal;
    friend class HmiZoneCapabilitiesMarshaller;
  };
  
}

#endif
