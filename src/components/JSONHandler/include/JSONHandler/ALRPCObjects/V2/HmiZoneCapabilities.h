#ifndef NSAPPLINKRPCV2_HMIZONECAPABILITIES_INCLUDE
#define NSAPPLINKRPCV2_HMIZONECAPABILITIES_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsAppLinkRPCV2
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
