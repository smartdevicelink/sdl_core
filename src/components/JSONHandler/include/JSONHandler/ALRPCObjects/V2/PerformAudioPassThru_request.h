#ifndef NSAPPLINKRPCV2_PERFORMAUDIOPASSTHRU_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_PERFORMAUDIOPASSTHRU_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "AudioCaptureQuality.h"
#include "AudioType.h"
#include "SamplingRate.h"
#include "TTSChunk.h"
#include "JSONHandler/ALRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  Starts audio pass thru session 

  class PerformAudioPassThru_request : public NsAppLinkRPC::ALRPCMessage
  {
  public:
  
    PerformAudioPassThru_request(const PerformAudioPassThru_request& c);
    PerformAudioPassThru_request(void);
    
    virtual ~PerformAudioPassThru_request(void);
  
    PerformAudioPassThru_request& operator =(const PerformAudioPassThru_request&);
  
    bool checkIntegrity(void);

    const std::vector<TTSChunk>& get_initialPrompt(void) const;
    const std::string* get_audioPassThruDisplayText1(void) const;
    const std::string* get_audioPassThruDisplayText2(void) const;
    const SamplingRate& get_samplingRate(void) const;
    unsigned int get_maxDuration(void) const;
    const AudioCaptureQuality& get_bitsPerSample(void) const;
    const AudioType& get_audioType(void) const;

    bool set_initialPrompt(const std::vector<TTSChunk>& initialPrompt_);
    void reset_audioPassThruDisplayText1(void);
    bool set_audioPassThruDisplayText1(const std::string& audioPassThruDisplayText1_);
    void reset_audioPassThruDisplayText2(void);
    bool set_audioPassThruDisplayText2(const std::string& audioPassThruDisplayText2_);
    bool set_samplingRate(const SamplingRate& samplingRate_);
    bool set_maxDuration(unsigned int maxDuration_);
    bool set_bitsPerSample(const AudioCaptureQuality& bitsPerSample_);
    bool set_audioType(const AudioType& audioType_);

  private:
  
    friend class PerformAudioPassThru_requestMarshaller;


/**
     SYNC will speak this prompt before opening the audio pass thru session
     An array of text chunks of type TTSChunk. See TTSChunk
     The array must have at least one item
*/
      std::vector<TTSChunk> initialPrompt;	//!<   [%s..%s] 

///  First line of text displayed during audio capture.
      std::string* audioPassThruDisplayText1;	//!< (500)

///  Second line of text displayed during audio capture.
      std::string* audioPassThruDisplayText2;	//!< (500)

///   This value shall be allowed at 8 khz or 16 or 22 or 44 khz.
      SamplingRate samplingRate;

///  The maximum duration of audio recording in milliseconds. 
      unsigned int maxDuration;	//!<  (0,1000000)

///  Specifies the quality the audio is recorded. Currently 8 bit or 16 bit.
      AudioCaptureQuality bitsPerSample;

///  Specifies the type of audio data being requested.
      AudioType audioType;
  };

}

#endif
