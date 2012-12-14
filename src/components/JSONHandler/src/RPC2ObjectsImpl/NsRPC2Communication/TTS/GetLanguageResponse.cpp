#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/TTS/GetLanguageResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::TTS
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::TTS;


GetLanguageResponse& GetLanguageResponse::operator =(const GetLanguageResponse& c)
{
  language=c.language;
  return *this;
}


GetLanguageResponse::~GetLanguageResponse(void)
{
}


GetLanguageResponse::GetLanguageResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_TTS__GETLANGUAGERESPONSE)
{
}


GetLanguageResponse::GetLanguageResponse(const GetLanguageResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_TTS__GETLANGUAGERESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


const NsAppLinkRPC::Language& GetLanguageResponse::get_language(void)
{
  return language;
}

bool GetLanguageResponse::set_language(const NsAppLinkRPC::Language& language_)
{
  language=language_;
  return true;
}

bool GetLanguageResponse::checkIntegrity(void)
{
  return GetLanguageResponseMarshaller::checkIntegrity(*this);
}
