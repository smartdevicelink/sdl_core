#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/PerformAudioPassThru.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


PerformAudioPassThru& PerformAudioPassThru::operator =(const PerformAudioPassThru& c)
{
  initialPrompt=c.initialPrompt;
  if(audioPassThruDisplayText1)  delete audioPassThruDisplayText1;
  audioPassThruDisplayText1= c.audioPassThruDisplayText1 ? new std::string(c.audioPassThruDisplayText1[0]) : 0;
  if(audioPassThruDisplayText2)  delete audioPassThruDisplayText2;
  audioPassThruDisplayText2= c.audioPassThruDisplayText2 ? new std::string(c.audioPassThruDisplayText2[0]) : 0;
  samplingRate=c.samplingRate;
  maxDuration=c.maxDuration;
  bitsPerSample=c.bitsPerSample;
  audioType=c.audioType;
  appId=c.appId;
  return *this;
}


PerformAudioPassThru::~PerformAudioPassThru(void)
{
  if(audioPassThruDisplayText1)  delete audioPassThruDisplayText1;
  if(audioPassThruDisplayText2)  delete audioPassThruDisplayText2;
}


PerformAudioPassThru::PerformAudioPassThru(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__PERFORMAUDIOPASSTHRU),
  audioPassThruDisplayText1(0),
  audioPassThruDisplayText2(0)
{
}


PerformAudioPassThru::PerformAudioPassThru(const PerformAudioPassThru& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__PERFORMAUDIOPASSTHRU,c.getId())
{
  *this=c;
}


const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>& PerformAudioPassThru::get_initialPrompt(void)
{
  return initialPrompt;
}

bool PerformAudioPassThru::set_initialPrompt(const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>& initialPrompt_)
{
  initialPrompt=initialPrompt_;
  return true;
}

const std::string* PerformAudioPassThru::get_audioPassThruDisplayText1(void)
{
  return audioPassThruDisplayText1;
}

bool PerformAudioPassThru::set_audioPassThruDisplayText1(const std::string& audioPassThruDisplayText1_)
{
  if(audioPassThruDisplayText1)  delete audioPassThruDisplayText1;
  audioPassThruDisplayText1=new std::string(audioPassThruDisplayText1_);
  return true;
}

void PerformAudioPassThru::reset_audioPassThruDisplayText1(void)
{
  if(audioPassThruDisplayText1)  delete audioPassThruDisplayText1;
  audioPassThruDisplayText1=0;
}

const std::string* PerformAudioPassThru::get_audioPassThruDisplayText2(void)
{
  return audioPassThruDisplayText2;
}

bool PerformAudioPassThru::set_audioPassThruDisplayText2(const std::string& audioPassThruDisplayText2_)
{
  if(audioPassThruDisplayText2)  delete audioPassThruDisplayText2;
  audioPassThruDisplayText2=new std::string(audioPassThruDisplayText2_);
  return true;
}

void PerformAudioPassThru::reset_audioPassThruDisplayText2(void)
{
  if(audioPassThruDisplayText2)  delete audioPassThruDisplayText2;
  audioPassThruDisplayText2=0;
}

const NsSmartDeviceLinkRPCV2::SamplingRate& PerformAudioPassThru::get_samplingRate(void)
{
  return samplingRate;
}

bool PerformAudioPassThru::set_samplingRate(const NsSmartDeviceLinkRPCV2::SamplingRate& samplingRate_)
{
  samplingRate=samplingRate_;
  return true;
}

unsigned int PerformAudioPassThru::get_maxDuration(void)
{
  return maxDuration;
}

bool PerformAudioPassThru::set_maxDuration(unsigned int maxDuration_)
{
  maxDuration=maxDuration_;
  return true;
}

const NsSmartDeviceLinkRPCV2::AudioCaptureQuality& PerformAudioPassThru::get_bitsPerSample(void)
{
  return bitsPerSample;
}

bool PerformAudioPassThru::set_bitsPerSample(const NsSmartDeviceLinkRPCV2::AudioCaptureQuality& bitsPerSample_)
{
  bitsPerSample=bitsPerSample_;
  return true;
}

const NsSmartDeviceLinkRPCV2::AudioType& PerformAudioPassThru::get_audioType(void)
{
  return audioType;
}

bool PerformAudioPassThru::set_audioType(const NsSmartDeviceLinkRPCV2::AudioType& audioType_)
{
  audioType=audioType_;
  return true;
}

int PerformAudioPassThru::get_appId(void)
{
  return appId;
}

bool PerformAudioPassThru::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool PerformAudioPassThru::checkIntegrity(void)
{
  return PerformAudioPassThruMarshaller::checkIntegrity(*this);
}
