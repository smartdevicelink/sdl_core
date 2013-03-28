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

#ifndef NSSMARTDEVICELINKKRPC_PERFORMINTERACTION_REQUEST_INCLUDE
#define NSSMARTDEVICELINKKRPC_PERFORMINTERACTION_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "InteractionMode.h"
#include "TTSChunk.h"
#include "JSONHandler/SDLRPCRequest.h"


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

///  Triggers an interaction (e.g. "Permit GPS?" - Yes, no, Always Allow).

  class PerformInteraction_request : public SDLRPCRequest
  {
  public:
  
    PerformInteraction_request(const PerformInteraction_request& c);
    PerformInteraction_request(void);
    
    virtual ~PerformInteraction_request(void);
  
    PerformInteraction_request& operator =(const PerformInteraction_request&);
  
    bool checkIntegrity(void);

    const std::string& get_initialText(void) const;
    const std::vector<TTSChunk>& get_initialPrompt(void) const;
    const InteractionMode& get_interactionMode(void) const;
    const std::vector<unsigned int>& get_interactionChoiceSetIDList(void) const;
    const std::vector<TTSChunk>* get_helpPrompt(void) const;
    const std::vector<TTSChunk>* get_timeoutPrompt(void) const;
    const unsigned int* get_timeout(void) const;

    bool set_initialText(const std::string& initialText_);
    bool set_initialPrompt(const std::vector<TTSChunk>& initialPrompt_);
    bool set_interactionMode(const InteractionMode& interactionMode_);
    bool set_interactionChoiceSetIDList(const std::vector<unsigned int>& interactionChoiceSetIDList_);
    void reset_helpPrompt(void);
    bool set_helpPrompt(const std::vector<TTSChunk>& helpPrompt_);
    void reset_timeoutPrompt(void);
    bool set_timeoutPrompt(const std::vector<TTSChunk>& timeoutPrompt_);
    void reset_timeout(void);
    bool set_timeout(unsigned int timeout_);

  private:
  
    friend class PerformInteraction_requestMarshaller;


///  Text to be displayed first.
      std::string initialText;	//!< (500)

/**
     This is the intial prompt spoken to the user at the start of an interaction
     An array of text chunks of type TTSChunk. See TTSChunk
     The array must have at least one item
*/
      std::vector<TTSChunk> initialPrompt;	//!<   [%s..%s] 

///  See InteractionMode.
      InteractionMode interactionMode;

///  List of interaction choice set IDs to use with an interaction.
      std::vector<unsigned int> interactionChoiceSetIDList;	//!<   [%s..%s]  (0,2000000000)

/**
     Help text. This is the spoken string when a user speaks "help" when the interaction is occuring.
     An array of text chunks of type TTSChunk. See TTSChunk
     The array must have at least one item
*/
      std::vector<TTSChunk>* helpPrompt;	//!<   [%s..%s] 

/**
     Timeout text. This text is spoken when a VR interaction times out.
     An array of text chunks of type TTSChunk. See TTSChunk
     The array must have at least one item
*/
      std::vector<TTSChunk>* timeoutPrompt;	//!<   [%s..%s] 

/**
     Timeout in milliseconds.
     If omitted a standard value of 10000 milliseconds is used.
*/
      unsigned int* timeout;	//!<  (5000,100000)
  };

}

#endif
