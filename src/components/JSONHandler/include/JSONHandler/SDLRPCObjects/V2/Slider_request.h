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

#ifndef NSSMARTDEVICELINKKRPCV2_SLIDER_REQUEST_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_SLIDER_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "JSONHandler/SDLRPCMessage.h"


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

///  Creates a full screen or pop-up overlay (depending on platform) with a single user controlled slider.

  class Slider_request : public NsSmartDeviceLinkRPC::SDLRPCMessage
  {
  public:
  
    Slider_request(const Slider_request& c);
    Slider_request(void);
    
    virtual ~Slider_request(void);
  
    Slider_request& operator =(const Slider_request&);
  
    bool checkIntegrity(void);

    unsigned int get_numTicks(void) const;
    unsigned int get_position(void) const;
    const std::string& get_sliderHeader(void) const;
    const std::vector<std::string>* get_sliderFooter(void) const;
    unsigned int get_timeout(void) const;

    bool set_numTicks(unsigned int numTicks_);
    bool set_position(unsigned int position_);
    bool set_sliderHeader(const std::string& sliderHeader_);
    void reset_sliderFooter(void);
    bool set_sliderFooter(const std::vector<std::string>& sliderFooter_);
    bool set_timeout(unsigned int timeout_);

  private:
  
    friend class Slider_requestMarshaller;


///  Number of selectable items on a horizontal axis
      unsigned int numTicks;	//!<  (2,26)

///  Initial position of slider control (cannot exceed numTicks)
      unsigned int position;	//!<  (1,26)

///  Text header to display
      std::string sliderHeader;	//!< (500)

/**
     Text footer to display (meant to display min/max threshold descriptors).
     For a static text footer, only one footer string shall be provided in the array.
     For a dynamic text footer, the number of footer text string in the array must match the numTicks value.
     For a dynamic text footer, text array string should correlate with potential slider position index.
     If omitted on supported displays, no footer text shall be displayed.
*/
      std::vector<std::string>* sliderFooter;	//!<   [%s..%s] (500)

///  App defined timeout.
      unsigned int timeout;	//!<  (0,65535)
  };

}

#endif
