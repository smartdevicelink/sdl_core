#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/TTS/SpeakResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::TTS
  version	1.2
  generated at	Tue Nov 20 13:32:23 2012
  source stamp	Mon Nov 19 10:17:20 2012
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
