#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/ChangeLanguageRegistrationResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 15:06:30 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


ChangeLanguageRegistrationResponse& ChangeLanguageRegistrationResponse::operator =(const ChangeLanguageRegistrationResponse& c)
{
  return *this;
}


ChangeLanguageRegistrationResponse::~ChangeLanguageRegistrationResponse(void)
{
}


ChangeLanguageRegistrationResponse::ChangeLanguageRegistrationResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__CHANGELANGUAGEREGISTRATIONRESPONSE)
{
}


ChangeLanguageRegistrationResponse::ChangeLanguageRegistrationResponse(const ChangeLanguageRegistrationResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__CHANGELANGUAGEREGISTRATIONRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool ChangeLanguageRegistrationResponse::checkIntegrity(void)
{
  return ChangeLanguageRegistrationResponseMarshaller::checkIntegrity(*this);
}
