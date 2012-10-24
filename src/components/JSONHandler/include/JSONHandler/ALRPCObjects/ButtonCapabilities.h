#ifndef BUTTONCAPABILITIES_INCLUDE
#define BUTTONCAPABILITIES_INCLUDE


#include "ButtonName.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/



///  Contains information about a button's capabilities.

class ButtonCapabilities
{
public:

  ButtonCapabilities(const ButtonCapabilities& c);
  ButtonCapabilities(void);

  bool checkIntegrity(void);
// getters

  bool get_longPressAvailable(void) const;
  const ButtonName& get_name(void) const;
  bool get_shortPressAvailable(void) const;
  bool get_upDownAvailable(void) const;

// setters

  bool set_longPressAvailable(bool longPressAvailable_);
  bool set_name(const ButtonName& name_);
  bool set_shortPressAvailable(bool shortPressAvailable_);
  bool set_upDownAvailable(bool upDownAvailable_);

private:

  friend class ButtonCapabilitiesMarshaller;

  bool longPressAvailable;
  ButtonName name;
  bool shortPressAvailable;
  bool upDownAvailable;
};

#endif
