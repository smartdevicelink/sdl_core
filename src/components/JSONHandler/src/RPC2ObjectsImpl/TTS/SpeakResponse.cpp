#include "../include/JSONHandler/RPC2Objects/TTS/SpeakResponse.h"
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


SpeakResponse& SpeakResponse::operator =(const SpeakResponse& c)
{
  return *this;
}


SpeakResponse::~SpeakResponse(void)
{
}


SpeakResponse::SpeakResponse(void) : 
  RPC2Response(Marshaller::METHOD_SPEAKRESPONSE)
{
}


SpeakResponse::SpeakResponse(const SpeakResponse& c) : RPC2Response(Marshaller::METHOD_SPEAKRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool SpeakResponse::checkIntegrity(void)
{
  return SpeakResponseMarshaller::checkIntegrity(*this);
}
