#ifndef NSAPPLINKRPC_SPEAK_REQUEST_INCLUDE
#define NSAPPLINKRPC_SPEAK_REQUEST_INCLUDE

#include <vector>

#include "TTSChunk.h"
#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
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
