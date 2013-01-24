#ifndef NSAPPLINKRPCV2_SPEAK_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_SPEAK_REQUEST_INCLUDE

#include <vector>

#include "TTSChunk.h"
#include "JSONHandler/ALRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  Speaks a text.

  class Speak_request : public NsAppLinkRPC::ALRPCMessage
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
