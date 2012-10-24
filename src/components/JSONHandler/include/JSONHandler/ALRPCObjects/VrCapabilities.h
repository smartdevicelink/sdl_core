#ifndef VRCAPABILITIES_INCLUDE
#define VRCAPABILITIES_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


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

#endif
