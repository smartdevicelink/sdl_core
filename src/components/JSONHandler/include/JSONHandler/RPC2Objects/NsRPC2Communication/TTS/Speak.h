#ifndef NSRPC2COMMUNICATION_TTS_SPEAK_INCLUDE
#define NSRPC2COMMUNICATION_TTS_SPEAK_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/TTSChunk.h"

/*
  interface	NsRPC2Communication::TTS
  version	1.2
  generated at	Thu Nov 29 14:32:09 2012
  source stamp	Thu Nov 29 14:32:05 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace TTS
  {

    class Speak : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      Speak(const Speak& c);
      Speak(void);
    
      Speak& operator =(const Speak&);
    
      virtual ~Speak(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::vector< NsAppLinkRPC::TTSChunk>& get_ttsChunks(void);

      int get_appId(void);


// setters
/// 1 <= size <= 100
      bool set_ttsChunks(const std::vector< NsAppLinkRPC::TTSChunk>& ttsChunks);

      bool set_appId(int appId);


    private:

      friend class SpeakMarshaller;

      std::vector< NsAppLinkRPC::TTSChunk> ttsChunks;
      int appId;

    };
  }
}

#endif
