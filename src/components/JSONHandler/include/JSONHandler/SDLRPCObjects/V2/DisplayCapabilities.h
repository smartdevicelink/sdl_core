//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_DISPLAYCAPABILITIES_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_DISPLAYCAPABILITIES_INCLUDE

#include <vector>

#include "DisplayType.h"
#include "MediaClockFormat.h"
#include "TextField.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/


namespace NsSmartDeviceLinkRPCV2
{

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

}

#endif
