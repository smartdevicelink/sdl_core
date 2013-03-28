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

#ifndef NSRPC2COMMUNICATION_UI_SETGLOBALPROPERTIES_INCLUDE
#define NSRPC2COMMUNICATION_UI_SETGLOBALPROPERTIES_INCLUDE

#include <string>
#include <vector>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/TTSChunk.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/TTSChunk.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/VrHelpItem.h"

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

    class SetGlobalProperties : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      SetGlobalProperties(const SetGlobalProperties& c);
      SetGlobalProperties(void);
    
      SetGlobalProperties& operator =(const SetGlobalProperties&);
    
      virtual ~SetGlobalProperties(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>* get_helpPrompt(void);
      const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>* get_timeoutPrompt(void);
      const std::string* get_vrHelpTitle(void);
      const std::vector< NsSmartDeviceLinkRPCV2::VrHelpItem>* get_vrHelp(void);
      int get_appId(void);


// setters
/// 1 <= size <= 100
      bool set_helpPrompt(const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>& helpPrompt);

      void reset_helpPrompt(void);

/// 1 <= size <= 100
      bool set_timeoutPrompt(const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>& timeoutPrompt);

      void reset_timeoutPrompt(void);

/// vrHelpTitle <= 500
      bool set_vrHelpTitle(const std::string& vrHelpTitle);

      void reset_vrHelpTitle(void);

/// 1 <= size <= 100
      bool set_vrHelp(const std::vector< NsSmartDeviceLinkRPCV2::VrHelpItem>& vrHelp);

      void reset_vrHelp(void);

      bool set_appId(int appId);


    private:

      friend class SetGlobalPropertiesMarshaller;

      std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>* helpPrompt;
      std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>* timeoutPrompt;
      std::string* vrHelpTitle;
      std::vector< NsSmartDeviceLinkRPCV2::VrHelpItem>* vrHelp;
      int appId;

    };
  }
}

#endif
