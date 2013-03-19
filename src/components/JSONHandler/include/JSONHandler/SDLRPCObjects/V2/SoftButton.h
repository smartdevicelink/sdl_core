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

#ifndef NSSMARTDEVICELINKKRPCV2_SOFTBUTTON_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_SOFTBUTTON_INCLUDE

#include <string>

#include "Image.h"
#include "SystemAction.h"
#include "SoftButtonType.h"


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

  class SoftButton
  {
  public:
  
    SoftButton(const SoftButton& c);
    SoftButton(void);
  
    bool checkIntegrity(void);
  
    ~SoftButton(void);
    SoftButton& operator =(const SoftButton&);

// getters

    const Image* get_image(void) const;
    bool get_isHighlighted(void) const;
    unsigned int get_softButtonID(void) const;
    const SystemAction& get_systemAction(void) const;
    const std::string* get_text(void) const;
    const SoftButtonType& get_type(void) const;

// setters

    void reset_image(void);
    bool set_image(const Image& image_);
    bool set_isHighlighted(bool isHighlighted_);
    bool set_softButtonID(unsigned int softButtonID_);
    bool set_systemAction(const SystemAction& systemAction_);
    void reset_text(void);
    bool set_text(const std::string& text_);
    bool set_type(const SoftButtonType& type_);

  private:

    friend class SoftButtonMarshaller;


///  Optional image struct for SoftButton (if defined as IMAGE or BOTH)
      Image* image;

/**
     True, if highlighted
     False, if not highlighted
*/
      bool isHighlighted;

///  Value which is returned via OnButtonPress / OnButtonEvent
      unsigned int softButtonID;	//!<  (0,65536)

///  Parameter indicating whether selecting a SoftButton shall call a specific system action.  This is intended to allow Notifications to bring the callee into full / focus; or in the case of persistent overlays, the overlay can persist when a SoftButton is pressed.
      SystemAction systemAction;

///  Optional text to display (if defined as TEXT or BOTH)
      std::string* text;	//!< (500)

///  Describes, whether it is text, highlighted text, icon, or dynamic image. See softButtonType
      SoftButtonType type;
  };

}

#endif
