//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPC_SETGLOBALPROPERTIES_REQUEST_INCLUDE
#define NSSMARTDEVICELINKKRPC_SETGLOBALPROPERTIES_REQUEST_INCLUDE

#include <vector>

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

///  Allows setting global properties.

  class SetGlobalProperties_request : public SDLRPCRequest
  {
  public:
  
    SetGlobalProperties_request(const SetGlobalProperties_request& c);
    SetGlobalProperties_request(void);
    
    virtual ~SetGlobalProperties_request(void);
  
    SetGlobalProperties_request& operator =(const SetGlobalProperties_request&);
  
    bool checkIntegrity(void);

    const std::vector<TTSChunk>* get_helpPrompt(void) const;
    const std::vector<TTSChunk>* get_timeoutPrompt(void) const;

    void reset_helpPrompt(void);
    bool set_helpPrompt(const std::vector<TTSChunk>& helpPrompt_);
    void reset_timeoutPrompt(void);
    bool set_timeoutPrompt(const std::vector<TTSChunk>& timeoutPrompt_);

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
  };

}

#endif
