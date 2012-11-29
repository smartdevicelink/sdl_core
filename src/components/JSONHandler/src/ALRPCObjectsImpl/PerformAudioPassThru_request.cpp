#include "../include/JSONHandler/ALRPCObjects/PerformAudioPassThru_request.h"
#include "PerformAudioPassThru_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "AudioCaptureQuality_v2Marshaller.h"
#include "AudioTypeMarshaller.h"
#include "SamplingRateMarshaller.h"
#include "TTSChunk_v2Marshaller.h"

#define PROTOCOL_VERSION	42


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;
PerformAudioPassThru_request& PerformAudioPassThru_request::operator =(const PerformAudioPassThru_request& c)
{
  initialPrompt= c.initialPrompt;
  audioPassThruDisplayText1= c.audioPassThruDisplayText1 ? new std::string(c.audioPassThruDisplayText1[0]) : 0;
  audioPassThruDisplayText2= c.audioPassThruDisplayText2 ? new std::string(c.audioPassThruDisplayText2[0]) : 0;
  samplingRate= c.samplingRate ? new SamplingRate(c.samplingRate[0]) : 0;
  maxDuration= c.maxDuration ? new unsigned int(c.maxDuration[0]) : 0;
  bitsPerSample= c.bitsPerSample ? new AudioCaptureQuality_v2(c.bitsPerSample[0]) : 0;
  audioType= c.audioType ? new AudioType(c.audioType[0]) : 0;

  return *this;}


PerformAudioPassThru_request::~PerformAudioPassThru_request(void)
{
  if(audioPassThruDisplayText1)
    delete audioPassThruDisplayText1;
  if(audioPassThruDisplayText2)
    delete audioPassThruDisplayText2;
  if(samplingRate)
    delete samplingRate;
  if(maxDuration)
    delete maxDuration;
  if(bitsPerSample)
    delete bitsPerSample;
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
    samplingRate(0),
    maxDuration(0),
    bitsPerSample(0),
    audioType(0)
{
}



bool PerformAudioPassThru_request::set_initialPrompt(const std::vector<TTSChunk_v2>& initialPrompt_)
{
  unsigned int i=initialPrompt_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!TTSChunk_v2Marshaller::checkIntegrityConst(initialPrompt_[i]))   return false;
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

bool PerformAudioPassThru_request::set_samplingRate(const SamplingRate& samplingRate_)
{
  if(!SamplingRateMarshaller::checkIntegrityConst(samplingRate_))   return false;
  delete samplingRate;
  samplingRate=0;

  samplingRate=new SamplingRate(samplingRate_);
  return true;
}

void PerformAudioPassThru_request::reset_samplingRate(void)
{
  if(samplingRate)
    delete samplingRate;
  samplingRate=0;
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

bool PerformAudioPassThru_request::set_bitsPerSample(const AudioCaptureQuality_v2& bitsPerSample_)
{
  if(!AudioCaptureQuality_v2Marshaller::checkIntegrityConst(bitsPerSample_))   return false;
  delete bitsPerSample;
  bitsPerSample=0;

  bitsPerSample=new AudioCaptureQuality_v2(bitsPerSample_);
  return true;
}

void PerformAudioPassThru_request::reset_bitsPerSample(void)
{
  if(bitsPerSample)
    delete bitsPerSample;
  bitsPerSample=0;
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




const std::vector<TTSChunk_v2>& PerformAudioPassThru_request::get_initialPrompt(void) const 
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

const SamplingRate* PerformAudioPassThru_request::get_samplingRate(void) const 
{
  return samplingRate;
}

const unsigned int* PerformAudioPassThru_request::get_maxDuration(void) const 
{
  return maxDuration;
}

const AudioCaptureQuality_v2* PerformAudioPassThru_request::get_bitsPerSample(void) const 
{
  return bitsPerSample;
}

const AudioType* PerformAudioPassThru_request::get_audioType(void) const 
{
  return audioType;
}

