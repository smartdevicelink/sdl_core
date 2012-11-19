#ifndef NSAPPLINKRPC_SPEAK_REQUEST_INCLUDE
#define NSAPPLINKRPC_SPEAK_REQUEST_INCLUDE

#include <vector>

#include "TTSChunk.h"
#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Mon Nov 19 10:37:06 2012
  source stamp	Mon Nov 19 10:35:56 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

///  Speaks a text.

  class Speak_request : public ALRPCRequest
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
