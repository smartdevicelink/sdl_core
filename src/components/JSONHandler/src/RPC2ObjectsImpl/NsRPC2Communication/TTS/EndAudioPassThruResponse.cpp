#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/TTS/EndAudioPassThruResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::TTS
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::TTS;


EndAudioPassThruResponse& EndAudioPassThruResponse::operator =(const EndAudioPassThruResponse& c)
{
  return *this;
}


EndAudioPassThruResponse::~EndAudioPassThruResponse(void)
{
}


EndAudioPassThruResponse::EndAudioPassThruResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_TTS__ENDAUDIOPASSTHRURESPONSE)
{
}


EndAudioPassThruResponse::EndAudioPassThruResponse(const EndAudioPassThruResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_TTS__ENDAUDIOPASSTHRURESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool EndAudioPassThruResponse::checkIntegrity(void)
{
  return EndAudioPassThruResponseMarshaller::checkIntegrity(*this);
}
