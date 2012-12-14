#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/ChangeRegistrationResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


ChangeRegistrationResponse& ChangeRegistrationResponse::operator =(const ChangeRegistrationResponse& c)
{
  return *this;
}


ChangeRegistrationResponse::~ChangeRegistrationResponse(void)
{
}


ChangeRegistrationResponse::ChangeRegistrationResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__CHANGEREGISTRATIONRESPONSE)
{
}


ChangeRegistrationResponse::ChangeRegistrationResponse(const ChangeRegistrationResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__CHANGEREGISTRATIONRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool ChangeRegistrationResponse::checkIntegrity(void)
{
  return ChangeRegistrationResponseMarshaller::checkIntegrity(*this);
}
