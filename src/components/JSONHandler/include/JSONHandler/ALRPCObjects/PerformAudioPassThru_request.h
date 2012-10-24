#ifndef PERFORMAUDIOPASSTHRU_REQUEST_INCLUDE
#define PERFORMAUDIOPASSTHRU_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "AudioCaptureQuality.h"
#include "AudioType.h"
#include "TTSChunk.h"
#include "../../../JSONHandler/ALRPCRequest.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


///  Starts audio pass thru session 

class PerformAudioPassThru_request : public ALRPCRequest
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
  const unsigned int* get_bitRate(void) const;
  const unsigned int* get_maxDuration(void) const;
  const AudioCaptureQuality* get_audioQuality(void) const;
  const AudioType* get_audioType(void) const;

  bool set_initialPrompt(const std::vector<TTSChunk>& initialPrompt_);
  void reset_audioPassThruDisplayText1(void);
  bool set_audioPassThruDisplayText1(const std::string& audioPassThruDisplayText1_);
  void reset_audioPassThruDisplayText2(void);
  bool set_audioPassThruDisplayText2(const std::string& audioPassThruDisplayText2_);
  void reset_bitRate(void);
  bool set_bitRate(unsigned int bitRate_);
  void reset_maxDuration(void);
  bool set_maxDuration(unsigned int maxDuration_);
  void reset_audioQuality(void);
  bool set_audioQuality(const AudioCaptureQuality& audioQuality_);
  void reset_audioType(void);
  bool set_audioType(const AudioType& audioType_);

private:

  friend class PerformAudioPassThru_requestMarshaller;

  std::vector<TTSChunk> initialPrompt;	//!<   [%s..%s] 
  std::string* audioPassThruDisplayText1;	//!< (500)
  std::string* audioPassThruDisplayText2;	//!< (500)
  unsigned int* bitRate;	//!<  (8,44)
  unsigned int* maxDuration;	//!<  (0,1000000)
  AudioCaptureQuality* audioQuality;
  AudioType* audioType;
};

#endif
