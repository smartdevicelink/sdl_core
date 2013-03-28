//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/PerformAudioPassThru.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
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
