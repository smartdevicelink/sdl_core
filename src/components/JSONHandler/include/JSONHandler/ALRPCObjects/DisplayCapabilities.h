#ifndef DISPLAYCAPABILITIES_INCLUDE
#define DISPLAYCAPABILITIES_INCLUDE

#include <vector>

#include "DisplayType.h"
#include "MediaClockFormat.h"
#include "TextField.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/



///  Contains information about the display capabilities.

class DisplayCapabilities
{
public:

  DisplayCapabilities(const DisplayCapabilities& c);
  DisplayCapabilities(void);

  bool checkIntegrity(void);
// getters

  const DisplayType& get_displayType(void) const;
  const std::vector<MediaClockFormat>& get_mediaClockFormats(void) const;
  const std::vector<TextField>& get_textFields(void) const;

// setters

  bool set_displayType(const DisplayType& displayType_);
  bool set_mediaClockFormats(const std::vector<MediaClockFormat>& mediaClockFormats_);
  bool set_textFields(const std::vector<TextField>& textFields_);

private:

  friend class DisplayCapabilitiesMarshaller;

  DisplayType displayType;
  std::vector<MediaClockFormat> mediaClockFormats;	//!<   [%s..%s] 
  std::vector<TextField> textFields;	//!<   [%s..%s] 
};

#endif
