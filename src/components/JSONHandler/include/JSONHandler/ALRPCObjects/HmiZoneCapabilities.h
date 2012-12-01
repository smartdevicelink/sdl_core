#ifndef NSAPPLINKRPC_HMIZONECAPABILITIES_INCLUDE
#define NSAPPLINKRPC_HMIZONECAPABILITIES_INCLUDE


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
