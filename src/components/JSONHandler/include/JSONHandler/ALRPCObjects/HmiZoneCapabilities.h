#ifndef HMIZONECAPABILITIES_INCLUDE
#define HMIZONECAPABILITIES_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
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
