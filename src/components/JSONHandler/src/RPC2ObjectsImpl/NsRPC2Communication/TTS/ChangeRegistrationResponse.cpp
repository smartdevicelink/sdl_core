#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/TTS/ChangeRegistrationResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::TTS
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::TTS;


ChangeRegistrationResponse& ChangeRegistrationResponse::operator =(const ChangeRegistrationResponse& c)
{
  return *this;
}


ChangeRegistrationResponse::~ChangeRegistrationResponse(void)
{
}


ChangeRegistrationResponse::ChangeRegistrationResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_TTS__CHANGEREGISTRATIONRESPONSE)
{
}


ChangeRegistrationResponse::ChangeRegistrationResponse(const ChangeRegistrationResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_TTS__CHANGEREGISTRATIONRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool ChangeRegistrationResponse::checkIntegrity(void)
{
  return ChangeRegistrationResponseMarshaller::checkIntegrity(*this);
}
