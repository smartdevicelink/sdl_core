#include "../include/JSONHandler/RPC2Objects/TTS/Speak.h"
#include "../include/JSONHandler/RPC2Objects/TTS/Marshaller.h"

/*
  interface	RPC2Communication::TTS
  version	1.2
  generated at	Wed Nov  7 11:25:55 2012
  source stamp	Wed Nov  7 09:34:03 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::TTS;


Speak& Speak::operator =(const Speak& c)
{
  ttsChunks=c.ttsChunks;
  return *this;
}


Speak::~Speak(void)
{
}


Speak::Speak(void) : 
  RPC2Request(Marshaller::METHOD_SPEAK)
{
}


Speak::Speak(const Speak& c) : RPC2Request(Marshaller::METHOD_SPEAK,c.getId())
{
  *this=c;
}


const std::vector< TTSChunk>& Speak::get_ttsChunks(void)
{
  return ttsChunks;
}

bool Speak::set_ttsChunks(const std::vector< TTSChunk>& ttsChunks_)
{
  ttsChunks=ttsChunks_;
  return true;
}

bool Speak::checkIntegrity(void)
{
  return SpeakMarshaller::checkIntegrity(*this);
}
