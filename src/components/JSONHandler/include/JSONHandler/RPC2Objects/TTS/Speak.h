#ifndef RPC2COMMUNICATION_TTS_SPEAK_INCLUDE
#define RPC2COMMUNICATION_TTS_SPEAK_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/TTSChunk.h"

/*
  interface	RPC2Communication::TTS
  version	1.2
  generated at	Wed Nov  7 11:25:55 2012
  source stamp	Wed Nov  7 09:34:03 2012
  author	robok0der
*/

namespace RPC2Communication
{
  namespace TTS
  {

    class Speak : public ::RPC2Communication::RPC2Request
    {
    public:
    
      Speak(const Speak& c);
      Speak(void);
    
      Speak& operator =(const Speak&);
    
      virtual ~Speak(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::vector< ::AppLinkRPC::TTSChunk>& get_ttsChunks(void);


// setters
/// 1 <= size <= 100
      bool set_ttsChunks(const std::vector< ::AppLinkRPC::TTSChunk>& ttsChunks);


    private:

      friend class SpeakMarshaller;

      std::vector< ::AppLinkRPC::TTSChunk> ttsChunks;

    };
  }
}

#endif
