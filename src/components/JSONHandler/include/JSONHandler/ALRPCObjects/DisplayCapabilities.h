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
  generated at	Tue Oct 30 08:29:32 2012
  source stamp	Thu Oct 25 06:49:27 2012
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


///  The type of the display. See DisplayType
    DisplayType displayType;

///  A set of all supported formats of the media clock. See MediaClockFormat
    std::vector<MediaClockFormat> mediaClockFormats;	//!<   [%s..%s] 

///  A set of all fields that support text data. See TextField
    std::vector<TextField> textFields;	//!<   [%s..%s] 
};

#endif
