//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPC_SPEAK_REQUEST_INCLUDE
#define NSSMARTDEVICELINKKRPC_SPEAK_REQUEST_INCLUDE

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

///  Speaks a text.

  class Speak_request : public SDLRPCRequest
  {
  public:
  
    Speak_request(const Speak_request& c);
    Speak_request(void);
    
    virtual ~Speak_request(void);
  
    bool checkIntegrity(void);

    const std::vector<TTSChunk>& get_ttsChunks(void) const;

    bool set_ttsChunks(const std::vector<TTSChunk>& ttsChunks_);

  private:
  
    friend class Speak_requestMarshaller;


/**
     An array of text chunks of type TTSChunk. See TTSChunk
     The array must have at least one item
*/
      std::vector<TTSChunk> ttsChunks;	//!<   [%s..%s] 
  };

}

#endif
