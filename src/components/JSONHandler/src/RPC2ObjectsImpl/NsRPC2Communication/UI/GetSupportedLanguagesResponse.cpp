#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/GetSupportedLanguagesResponse.h"
#include "JSONHandler/RPC2Objects/Marshaller.h"

using namespace NsRPC2Communication::UI;

GetSupportedLanguagesResponse& GetSupportedLanguagesResponse::operator =(const GetSupportedLanguagesResponse& c)
{
  languages=c.languages;
  return *this;
}


GetSupportedLanguagesResponse::~GetSupportedLanguagesResponse(void)
{
}


GetSupportedLanguagesResponse::GetSupportedLanguagesResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__GETSUPPORTEDLANGUAGESRESPONSE)
{
}


GetSupportedLanguagesResponse::GetSupportedLanguagesResponse(const GetSupportedLanguagesResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__GETSUPPORTEDLANGUAGESRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}

const std::vector<NsAppLinkRPC::Language>& GetSupportedLanguagesResponse::get_languages() const
{
    return languages;
}

bool GetSupportedLanguagesResponse::set_languages(const std::vector<NsAppLinkRPC::Language> & c)
{
    languages = c;
    return true;
}
