//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_TTS_SPEAK_INCLUDE
#define NSRPC2COMMUNICATION_TTS_SPEAK_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/TTSChunk.h"

/*
  interface	NsRPC2Communication::TTS
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
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
      const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>& get_ttsChunks(void);

      int get_appId(void);


// setters
/// 1 <= size <= 100
      bool set_ttsChunks(const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>& ttsChunks);

      bool set_appId(int appId);


    private:

      friend class SpeakMarshaller;

      std::vector< NsSmartDeviceLinkRPCV2::TTSChunk> ttsChunks;
      int appId;

    };
  }
}

#endif
