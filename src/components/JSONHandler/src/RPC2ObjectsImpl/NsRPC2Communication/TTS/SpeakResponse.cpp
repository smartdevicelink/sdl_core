#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/TTS/SpeakResponse.h"
#include "../../../../../Marshaller.h"

/*
  interface	NsRPC2Communication::TTS
  version	1.2
  generated at	Mon Nov 19 08:20:05 2012
  source stamp	Mon Nov 19 07:04:40 2012
  author	robok0der
*/

using namespace NsRPC2Communication::TTS;


SpeakResponse& SpeakResponse::operator =(const SpeakResponse& c)
{
  return *this;
}


SpeakResponse::~SpeakResponse(void)
{
}


SpeakResponse::SpeakResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_TTS__SPEAKRESPONSE)
{
}


SpeakResponse::SpeakResponse(const SpeakResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_TTS__SPEAKRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool SpeakResponse::checkIntegrity(void)
{
  return SpeakResponseMarshaller::checkIntegrity(*this);
}
