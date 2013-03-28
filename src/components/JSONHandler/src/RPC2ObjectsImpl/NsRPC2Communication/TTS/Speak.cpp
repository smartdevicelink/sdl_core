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

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/TTS/Speak.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::TTS
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::TTS;


Speak& Speak::operator =(const Speak& c)
{
  ttsChunks=c.ttsChunks;
  appId=c.appId;
  return *this;
}


Speak::~Speak(void)
{
}


Speak::Speak(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_TTS__SPEAK)
{
}


Speak::Speak(const Speak& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_TTS__SPEAK,c.getId())
{
  *this=c;
}


const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>& Speak::get_ttsChunks(void)
{
  return ttsChunks;
}

bool Speak::set_ttsChunks(const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>& ttsChunks_)
{
  ttsChunks=ttsChunks_;
  return true;
}

int Speak::get_appId(void)
{
  return appId;
}

bool Speak::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool Speak::checkIntegrity(void)
{
  return SpeakMarshaller::checkIntegrity(*this);
}
