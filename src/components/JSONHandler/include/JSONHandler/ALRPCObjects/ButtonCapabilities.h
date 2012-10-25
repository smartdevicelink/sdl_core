#ifndef BUTTONCAPABILITIES_INCLUDE
#define BUTTONCAPABILITIES_INCLUDE


#include "ButtonName.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/



///  Contains information about a buttons capabilities.

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
