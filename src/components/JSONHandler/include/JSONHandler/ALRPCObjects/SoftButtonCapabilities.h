#ifndef SOFTBUTTONCAPABILITIES_INCLUDE
#define SOFTBUTTONCAPABILITIES_INCLUDE




/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/



///  Contains information about a SoftButton's capabilities.

class SoftButtonCapabilities
{
public:

  SoftButtonCapabilities(const SoftButtonCapabilities& c);
  SoftButtonCapabilities(void);

  bool checkIntegrity(void);
// getters

  bool get_imageSupported(void) const;
  bool get_longPressAvailable(void) const;
  bool get_shortPressAvailable(void) const;
  bool get_upDownAvailable(void) const;

// setters

  bool set_imageSupported(bool imageSupported_);
  bool set_longPressAvailable(bool longPressAvailable_);
  bool set_shortPressAvailable(bool shortPressAvailable_);
  bool set_upDownAvailable(bool upDownAvailable_);

private:

  friend class SoftButtonCapabilitiesMarshaller;

  bool imageSupported;
  bool longPressAvailable;
  bool shortPressAvailable;
  bool upDownAvailable;
};

#endif
