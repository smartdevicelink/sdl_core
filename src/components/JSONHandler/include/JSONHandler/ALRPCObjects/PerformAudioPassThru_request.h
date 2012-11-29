#ifndef NSAPPLINKRPC_PERFORMAUDIOPASSTHRU_REQUEST_INCLUDE
#define NSAPPLINKRPC_PERFORMAUDIOPASSTHRU_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "AudioCaptureQuality_v2.h"
#include "AudioType.h"
#include "SamplingRate.h"
#include "TTSChunk_v2.h"
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

///  Starts audio pass thru session 

  class PerformAudioPassThru_request : public ALRPCRequest
  {
  public:
  
    PerformAudioPassThru_request(const PerformAudioPassThru_request& c);
    PerformAudioPassThru_request(void);
    
    virtual ~PerformAudioPassThru_request(void);
  
    PerformAudioPassThru_request& operator =(const PerformAudioPassThru_request&);
  
    bool checkIntegrity(void);

    const std::vector<TTSChunk_v2>& get_initialPrompt(void) const;
    const std::string* get_audioPassThruDisplayText1(void) const;
    const std::string* get_audioPassThruDisplayText2(void) const;
    const SamplingRate* get_samplingRate(void) const;
    const unsigned int* get_maxDuration(void) const;
    const AudioCaptureQuality_v2* get_bitsPerSample(void) const;
    const AudioType* get_audioType(void) const;

    bool set_initialPrompt(const std::vector<TTSChunk_v2>& initialPrompt_);
    void reset_audioPassThruDisplayText1(void);
    bool set_audioPassThruDisplayText1(const std::string& audioPassThruDisplayText1_);
    void reset_audioPassThruDisplayText2(void);
    bool set_audioPassThruDisplayText2(const std::string& audioPassThruDisplayText2_);
    void reset_samplingRate(void);
    bool set_samplingRate(const SamplingRate& samplingRate_);
    void reset_maxDuration(void);
    bool set_maxDuration(unsigned int maxDuration_);
    void reset_bitsPerSample(void);
    bool set_bitsPerSample(const AudioCaptureQuality_v2& bitsPerSample_);
    void reset_audioType(void);
    bool set_audioType(const AudioType& audioType_);

  private:
  
    friend class PerformAudioPassThru_requestMarshaller;


/**
     SYNC will speak this prompt before opening the audio pass thru session
     An array of text chunks of type TTSChunk. See TTSChunk
     The array must have at least one item
*/
      std::vector<TTSChunk_v2> initialPrompt;	//!<   [%s..%s] 

///  First line of text displayed during audio capture.
      std::string* audioPassThruDisplayText1;	//!< (500)

///  Second line of text displayed during audio capture.
      std::string* audioPassThruDisplayText2;	//!< (500)

///   This value shall be allowed at 8 khz or 16 or 22 or 44 khz.
      SamplingRate* samplingRate;

///  The maximum duration of audio recording in milliseconds. 
      unsigned int* maxDuration;	//!<  (0,1000000)

///  Specifies the quality the audio is recorded. Currently 8 bit or 16 bit.
      AudioCaptureQuality_v2* bitsPerSample;

///  Specifies the type of audio data being requested.
      AudioType* audioType;
  };

}

#endif
