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

#ifndef NSSMARTDEVICELINKKRPCV2_SCROLLABLEMESSAGE_REQUEST_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_SCROLLABLEMESSAGE_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "SoftButton.h"
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

///  Creates a full screen overlay containing a large block of formatted text that can be scrolled with up to 8 SoftButtons defined

  class ScrollableMessage_request : public NsSmartDeviceLinkRPC::SDLRPCMessage
  {
  public:
  
    ScrollableMessage_request(const ScrollableMessage_request& c);
    ScrollableMessage_request(void);
    
    virtual ~ScrollableMessage_request(void);
  
    ScrollableMessage_request& operator =(const ScrollableMessage_request&);
  
    bool checkIntegrity(void);

    const std::string& get_scrollableMessageBody(void) const;
    const unsigned int* get_timeout(void) const;
    const std::vector<SoftButton>* get_softButtons(void) const;

    bool set_scrollableMessageBody(const std::string& scrollableMessageBody_);
    void reset_timeout(void);
    bool set_timeout(unsigned int timeout_);
    void reset_softButtons(void);
    bool set_softButtons(const std::vector<SoftButton>& softButtons_);

  private:
  
    friend class ScrollableMessage_requestMarshaller;


///  Body of text that can include newlines and tabs.
      std::string scrollableMessageBody;	//!< (500)

///  App defined timeout.
      unsigned int* timeout;	//!<  (0,65535)

/**
     App defined SoftButtons.
     If omitted on supported displays, only the system defined "Close" SoftButton will be displayed.
*/
      std::vector<SoftButton>* softButtons;	//!<   [%s..%s] 
  };

}

#endif
