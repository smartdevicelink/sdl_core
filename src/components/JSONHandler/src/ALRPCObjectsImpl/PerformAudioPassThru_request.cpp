#include "../../include/JSONHandler/ALRPCObjects/PerformAudioPassThru_request.h"
#include "PerformAudioPassThru_requestMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "AudioCaptureQualityMarshaller.h"
#include "AudioTypeMarshaller.h"
#include "TTSChunkMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/

PerformAudioPassThru_request& PerformAudioPassThru_request::operator =(const PerformAudioPassThru_request& c)
{
  initialPrompt= c.initialPrompt;
  audioPassThruDisplayText1= c.audioPassThruDisplayText1 ? new std::string(c.audioPassThruDisplayText1[0]) : 0;
  audioPassThruDisplayText2= c.audioPassThruDisplayText2 ? new std::string(c.audioPassThruDisplayText2[0]) : 0;
  bitRate= c.bitRate ? new unsigned int(c.bitRate[0]) : 0;
  maxDuration= c.maxDuration ? new unsigned int(c.maxDuration[0]) : 0;
  audioQuality= c.audioQuality ? new AudioCaptureQuality(c.audioQuality[0]) : 0;
  audioType= c.audioType ? new AudioType(c.audioType[0]) : 0;

  return *this;}


PerformAudioPassThru_request::~PerformAudioPassThru_request(void)
{
  if(audioPassThruDisplayText1)
    delete audioPassThruDisplayText1;
  if(audioPassThruDisplayText2)
    delete audioPassThruDisplayText2;
  if(bitRate)
    delete bitRate;
  if(maxDuration)
    delete maxDuration;
  if(audioQuality)
    delete audioQuality;
  if(audioType)
    delete audioType;
}


PerformAudioPassThru_request::PerformAudioPassThru_request(const PerformAudioPassThru_request& c)
{
  *this=c;
}


bool PerformAudioPassThru_request::checkIntegrity(void)
{
  return PerformAudioPassThru_requestMarshaller::checkIntegrity(*this);
}


PerformAudioPassThru_request::PerformAudioPassThru_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_PERFORMAUDIOPASSTHRU_REQUEST),
      audioPassThruDisplayText1(0),
    audioPassThruDisplayText2(0),
    bitRate(0),
    maxDuration(0),
    audioQuality(0),
    audioType(0)
{
}



bool PerformAudioPassThru_request::set_initialPrompt(const std::vector<TTSChunk>& initialPrompt_)
{
  unsigned int i=initialPrompt_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!TTSChunkMarshaller::checkIntegrityConst(initialPrompt_[i]))   return false;
  }
  initialPrompt=initialPrompt_;
  return true;
}

bool PerformAudioPassThru_request::set_audioPassThruDisplayText1(const std::string& audioPassThruDisplayText1_)
{
  if(audioPassThruDisplayText1_.length()>500)  return false;
  delete audioPassThruDisplayText1;
  audioPassThruDisplayText1=0;

  audioPassThruDisplayText1=new std::string(audioPassThruDisplayText1_);
  return true;
}

void PerformAudioPassThru_request::reset_audioPassThruDisplayText1(void)
{
  if(audioPassThruDisplayText1)
    delete audioPassThruDisplayText1;
  audioPassThruDisplayText1=0;
}

bool PerformAudioPassThru_request::set_audioPassThruDisplayText2(const std::string& audioPassThruDisplayText2_)
{
  if(audioPassThruDisplayText2_.length()>500)  return false;
  delete audioPassThruDisplayText2;
  audioPassThruDisplayText2=0;

  audioPassThruDisplayText2=new std::string(audioPassThruDisplayText2_);
  return true;
}

void PerformAudioPassThru_request::reset_audioPassThruDisplayText2(void)
{
  if(audioPassThruDisplayText2)
    delete audioPassThruDisplayText2;
  audioPassThruDisplayText2=0;
}

bool PerformAudioPassThru_request::set_bitRate(unsigned int bitRate_)
{
  if(bitRate_>44)  return false;
  if(bitRate_<8)  return false;
  delete bitRate;
  bitRate=0;

  bitRate=new unsigned int(bitRate_);
  return true;
}

void PerformAudioPassThru_request::reset_bitRate(void)
{
  if(bitRate)
    delete bitRate;
  bitRate=0;
}

bool PerformAudioPassThru_request::set_maxDuration(unsigned int maxDuration_)
{
  if(maxDuration_>1000000)  return false;
  delete maxDuration;
  maxDuration=0;

  maxDuration=new unsigned int(maxDuration_);
  return true;
}

void PerformAudioPassThru_request::reset_maxDuration(void)
{
  if(maxDuration)
    delete maxDuration;
  maxDuration=0;
}

bool PerformAudioPassThru_request::set_audioQuality(const AudioCaptureQuality& audioQuality_)
{
  if(!AudioCaptureQualityMarshaller::checkIntegrityConst(audioQuality_))   return false;
  delete audioQuality;
  audioQuality=0;

  audioQuality=new AudioCaptureQuality(audioQuality_);
  return true;
}

void PerformAudioPassThru_request::reset_audioQuality(void)
{
  if(audioQuality)
    delete audioQuality;
  audioQuality=0;
}

bool PerformAudioPassThru_request::set_audioType(const AudioType& audioType_)
{
  if(!AudioTypeMarshaller::checkIntegrityConst(audioType_))   return false;
  delete audioType;
  audioType=0;

  audioType=new AudioType(audioType_);
  return true;
}

void PerformAudioPassThru_request::reset_audioType(void)
{
  if(audioType)
    delete audioType;
  audioType=0;
}




const std::vector<TTSChunk>& PerformAudioPassThru_request::get_initialPrompt(void) const 
{
  return initialPrompt;
}

const std::string* PerformAudioPassThru_request::get_audioPassThruDisplayText1(void) const 
{
  return audioPassThruDisplayText1;
}

const std::string* PerformAudioPassThru_request::get_audioPassThruDisplayText2(void) const 
{
  return audioPassThruDisplayText2;
}

const unsigned int* PerformAudioPassThru_request::get_bitRate(void) const 
{
  return bitRate;
}

const unsigned int* PerformAudioPassThru_request::get_maxDuration(void) const 
{
  return maxDuration;
}

const AudioCaptureQuality* PerformAudioPassThru_request::get_audioQuality(void) const 
{
  return audioQuality;
}

const AudioType* PerformAudioPassThru_request::get_audioType(void) const 
{
  return audioType;
}

