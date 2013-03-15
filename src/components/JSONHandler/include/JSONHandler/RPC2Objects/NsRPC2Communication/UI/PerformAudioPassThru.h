//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_PERFORMAUDIOPASSTHRU_INCLUDE
#define NSRPC2COMMUNICATION_UI_PERFORMAUDIOPASSTHRU_INCLUDE

#include <string>
#include <vector>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/TTSChunk.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/SamplingRate.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/AudioCaptureQuality.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/AudioType.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class PerformAudioPassThru : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      PerformAudioPassThru(const PerformAudioPassThru& c);
      PerformAudioPassThru(void);
    
      PerformAudioPassThru& operator =(const PerformAudioPassThru&);
    
      virtual ~PerformAudioPassThru(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>& get_initialPrompt(void);

      const std::string* get_audioPassThruDisplayText1(void);
      const std::string* get_audioPassThruDisplayText2(void);
      const NsSmartDeviceLinkRPCV2::SamplingRate& get_samplingRate(void);

      unsigned int get_maxDuration(void);

      const NsSmartDeviceLinkRPCV2::AudioCaptureQuality& get_bitsPerSample(void);

      const NsSmartDeviceLinkRPCV2::AudioType& get_audioType(void);

      int get_appId(void);


// setters
/// 1 <= size <= 100
      bool set_initialPrompt(const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>& initialPrompt);

/// audioPassThruDisplayText1 <= 500
      bool set_audioPassThruDisplayText1(const std::string& audioPassThruDisplayText1);

      void reset_audioPassThruDisplayText1(void);

/// audioPassThruDisplayText2 <= 500
      bool set_audioPassThruDisplayText2(const std::string& audioPassThruDisplayText2);

      void reset_audioPassThruDisplayText2(void);

      bool set_samplingRate(const NsSmartDeviceLinkRPCV2::SamplingRate& samplingRate);

/// maxDuration <= 1000000
      bool set_maxDuration(unsigned int maxDuration);

      bool set_bitsPerSample(const NsSmartDeviceLinkRPCV2::AudioCaptureQuality& bitsPerSample);

      bool set_audioType(const NsSmartDeviceLinkRPCV2::AudioType& audioType);

      bool set_appId(int appId);


    private:

      friend class PerformAudioPassThruMarshaller;

      std::vector< NsSmartDeviceLinkRPCV2::TTSChunk> initialPrompt;
      std::string* audioPassThruDisplayText1;
      std::string* audioPassThruDisplayText2;
      NsSmartDeviceLinkRPCV2::SamplingRate samplingRate;
      unsigned int maxDuration;
      NsSmartDeviceLinkRPCV2::AudioCaptureQuality bitsPerSample;
      NsSmartDeviceLinkRPCV2::AudioType audioType;
      int appId;

    };
  }
}

#endif
