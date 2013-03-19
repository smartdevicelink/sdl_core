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

#ifndef NSSMARTDEVICELINKKRPCV2_SOFTBUTTONCAPABILITIES_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_SOFTBUTTONCAPABILITIES_INCLUDE




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


///  The button supports referencing a static or dynamic image.
      bool imageSupported;

/**
     The button supports a LONG press.
     Whenever the button is pressed long, onButtonPressed( LONG) will be invoked.
*/
      bool longPressAvailable;

/**
     The button supports a short press.
     Whenever the button is pressed short, onButtonPressed( SHORT) will be invoked.
*/
      bool shortPressAvailable;

/**
     The button supports "button down" and "button up".
     Whenever the button is pressed, onButtonEvent( DOWN) will be invoked.
     Whenever the button is released, onButtonEvent( UP) will be invoked.
*/
      bool upDownAvailable;
  };

}

#endif
