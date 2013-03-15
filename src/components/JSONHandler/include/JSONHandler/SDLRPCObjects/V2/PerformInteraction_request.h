//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_PERFORMINTERACTION_REQUEST_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_PERFORMINTERACTION_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "InteractionMode.h"
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

///  Triggers an interaction (e.g. "Permit GPS?" - Yes, no, Always Allow).

  class PerformInteraction_request : public NsSmartDeviceLinkRPC::SDLRPCMessage
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
    const std::vector<VrHelpItem>* get_vrHelp(void) const;

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
    void reset_vrHelp(void);
    bool set_vrHelp(const std::vector<VrHelpItem>& vrHelp_);

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

/**
     Ability to send suggested VR Help Items to display on-screen during Perform Interaction.
     If omitted on supported displays, the default SYNC generated list of suggested choices shall be displayed.
*/
      std::vector<VrHelpItem>* vrHelp;	//!<   [%s..%s] 
  };

}

#endif
