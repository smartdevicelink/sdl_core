#ifndef RPC2COMMUNICATION_UI_GETSUPPORTEDLANGUAGESRESPONSE
#define RPC2COMMUNICATION_UI_GETSUPPORTEDLANGUAGESRESPONSE

#include <vector>
#include "JSONHandler/RPC2Response.h"

#include "JSONHandler/ALRPCObjects/V1/Language.h"

namespace NsRPC2Communication
{
  namespace UI
  {
    class GetSupportedLanguagesResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
        GetSupportedLanguagesResponse();
        GetSupportedLanguagesResponse(const GetSupportedLanguagesResponse&);
        ~GetSupportedLanguagesResponse();

        GetSupportedLanguagesResponse & operator=(const GetSupportedLanguagesResponse&);

        const std::vector<NsAppLinkRPC::Language>& get_languages() const;

        bool set_languages(const std::vector<NsAppLinkRPC::Language> &);
    private:
        std::vector<NsAppLinkRPC::Language> languages;
        friend class GetSupportedLanguagesResponseMarshaller;
    };
  }
}

#endif