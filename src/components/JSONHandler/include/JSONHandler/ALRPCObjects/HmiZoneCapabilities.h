#ifndef HMIZONECAPABILITIES_INCLUDE
#define HMIZONECAPABILITIES_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


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

#endif
