//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#ifndef NSSMARTDEVICELINKKRPC_DISPLAYCAPABILITIES_INCLUDE
#define NSSMARTDEVICELINKKRPC_DISPLAYCAPABILITIES_INCLUDE

#include <vector>

#include "DisplayType.h"
#include "MediaClockFormat.h"
#include "TextField.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/


namespace NsSmartDeviceLinkRPC
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
