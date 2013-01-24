#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VR/GetSupportedLanguagesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::VR
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::VR;


GetSupportedLanguagesResponse& GetSupportedLanguagesResponse::operator =(const GetSupportedLanguagesResponse& c)
{
  languages=c.languages;
  return *this;
}


GetSupportedLanguagesResponse::~GetSupportedLanguagesResponse(void)
{
}


GetSupportedLanguagesResponse::GetSupportedLanguagesResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_VR__GETSUPPORTEDLANGUAGESRESPONSE)
{
}


GetSupportedLanguagesResponse::GetSupportedLanguagesResponse(const GetSupportedLanguagesResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_VR__GETSUPPORTEDLANGUAGESRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


const std::vector< NsAppLinkRPCV2::Language>& GetSupportedLanguagesResponse::get_languages(void)
{
  return languages;
}

bool GetSupportedLanguagesResponse::set_languages(const std::vector< NsAppLinkRPCV2::Language>& languages_)
{
  languages=languages_;
  return true;
}

bool GetSupportedLanguagesResponse::checkIntegrity(void)
{
  return GetSupportedLanguagesResponseMarshaller::checkIntegrity(*this);
}
