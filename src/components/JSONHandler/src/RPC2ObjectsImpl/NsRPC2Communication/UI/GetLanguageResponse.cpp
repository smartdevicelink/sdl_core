#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/GetLanguageResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


GetLanguageResponse& GetLanguageResponse::operator =(const GetLanguageResponse& c)
{
  hmiDisplayLanguage=c.hmiDisplayLanguage;
  return *this;
}


GetLanguageResponse::~GetLanguageResponse(void)
{
}


GetLanguageResponse::GetLanguageResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__GETLANGUAGERESPONSE)
{
}


GetLanguageResponse::GetLanguageResponse(const GetLanguageResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__GETLANGUAGERESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


const NsAppLinkRPC::Language& GetLanguageResponse::get_hmiDisplayLanguage(void)
{
  return hmiDisplayLanguage;
}

bool GetLanguageResponse::set_hmiDisplayLanguage(const NsAppLinkRPC::Language& hmiDisplayLanguage_)
{
  hmiDisplayLanguage=hmiDisplayLanguage_;
  return true;
}

bool GetLanguageResponse::checkIntegrity(void)
{
  return GetLanguageResponseMarshaller::checkIntegrity(*this);
}
