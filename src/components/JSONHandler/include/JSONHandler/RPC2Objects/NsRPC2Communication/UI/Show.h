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

#ifndef NSRPC2COMMUNICATION_UI_SHOW_INCLUDE
#define NSRPC2COMMUNICATION_UI_SHOW_INCLUDE

#include <string>
#include <vector>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/TextAlignment.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Image.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/SoftButton.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class Show : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      Show(const Show& c);
      Show(void);
    
      Show& operator =(const Show&);
    
      virtual ~Show(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::string* get_mainField1(void);
      const std::string* get_mainField2(void);
      const std::string* get_mainField3(void);
      const std::string* get_mainField4(void);
      const NsSmartDeviceLinkRPCV2::TextAlignment* get_alignment(void);
      const std::string* get_statusBar(void);
      const std::string* get_mediaClock(void);
      const std::string* get_mediaTrack(void);
      const NsSmartDeviceLinkRPCV2::Image* get_graphic(void);
      const std::vector< NsSmartDeviceLinkRPCV2::SoftButton>* get_softButtons(void);
      const std::vector< std::string>* get_customPresets(void);
      int get_appId(void);


// setters
/// mainField1 <= 500
      bool set_mainField1(const std::string& mainField1);

      void reset_mainField1(void);

/// mainField2 <= 500
      bool set_mainField2(const std::string& mainField2);

      void reset_mainField2(void);

/// mainField3 <= 500
      bool set_mainField3(const std::string& mainField3);

      void reset_mainField3(void);

/// mainField4 <= 500
      bool set_mainField4(const std::string& mainField4);

      void reset_mainField4(void);

      bool set_alignment(const NsSmartDeviceLinkRPCV2::TextAlignment& alignment);

      void reset_alignment(void);

/// statusBar <= 500
      bool set_statusBar(const std::string& statusBar);

      void reset_statusBar(void);

/// mediaClock <= 500
      bool set_mediaClock(const std::string& mediaClock);

      void reset_mediaClock(void);

/// mediaTrack <= 500
      bool set_mediaTrack(const std::string& mediaTrack);

      void reset_mediaTrack(void);

      bool set_graphic(const NsSmartDeviceLinkRPCV2::Image& graphic);

      void reset_graphic(void);

/// 0 <= size <= 8
      bool set_softButtons(const std::vector< NsSmartDeviceLinkRPCV2::SoftButton>& softButtons);

      void reset_softButtons(void);

/// customPresets[] <= 500 ; 0 <= size <= 6
      bool set_customPresets(const std::vector< std::string>& customPresets);

      void reset_customPresets(void);

      bool set_appId(int appId);


    private:

      friend class ShowMarshaller;

      std::string* mainField1;
      std::string* mainField2;
      std::string* mainField3;
      std::string* mainField4;
      NsSmartDeviceLinkRPCV2::TextAlignment* alignment;
      std::string* statusBar;
      std::string* mediaClock;
      std::string* mediaTrack;
      NsSmartDeviceLinkRPCV2::Image* graphic;
      std::vector< NsSmartDeviceLinkRPCV2::SoftButton>* softButtons;
      std::vector< std::string>* customPresets;
      int appId;

    };
  }
}

#endif
