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

#ifndef NSSMARTDEVICELINKKRPCV2_SETGLOBALPROPERTIES_REQUEST_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_SETGLOBALPROPERTIES_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "TTSChunk.h"
#include "VrHelpItem.h"
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

///  Allows setting global properties.

  class SetGlobalProperties_request : public NsSmartDeviceLinkRPC::SDLRPCMessage
  {
  public:
  
    SetGlobalProperties_request(const SetGlobalProperties_request& c);
    SetGlobalProperties_request(void);
    
    virtual ~SetGlobalProperties_request(void);
  
    SetGlobalProperties_request& operator =(const SetGlobalProperties_request&);
  
    bool checkIntegrity(void);

    const std::vector<TTSChunk>* get_helpPrompt(void) const;
    const std::vector<TTSChunk>* get_timeoutPrompt(void) const;
    const std::string* get_vrHelpTitle(void) const;
    const std::vector<VrHelpItem>* get_vrHelp(void) const;

    void reset_helpPrompt(void);
    bool set_helpPrompt(const std::vector<TTSChunk>& helpPrompt_);
    void reset_timeoutPrompt(void);
    bool set_timeoutPrompt(const std::vector<TTSChunk>& timeoutPrompt_);
    void reset_vrHelpTitle(void);
    bool set_vrHelpTitle(const std::string& vrHelpTitle_);
    void reset_vrHelp(void);
    bool set_vrHelp(const std::vector<VrHelpItem>& vrHelp_);

  private:
  
    friend class SetGlobalProperties_requestMarshaller;


/**
     The help prompt.
     An array of text chunks of type TTSChunk. See TTSChunk
     The array must have at least one item
*/
      std::vector<TTSChunk>* helpPrompt;	//!<   [%s..%s] 

/**
     Help text for a wait timeout.
     An array of text chunks of type TTSChunk. See TTSChunk
     The array must have at least one item
*/
      std::vector<TTSChunk>* timeoutPrompt;	//!<   [%s..%s] 

/**
     VR Help Title text.
     If omitted on supported displays, the default SYNC help title shall be used.
*/
      std::string* vrHelpTitle;	//!< (500)

/**
     VR Help Items.
     If omitted on supported displays, the default SYNC generated help items shall be used.
     If the list of VR Help Items contains nonsequential positions (e.g. [1,2,4]), the RPC shall be rejected.
*/
      std::vector<VrHelpItem>* vrHelp;	//!<   [%s..%s] 
  };

}

#endif
