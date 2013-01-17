#ifndef NSRPC2COMMUNICATION_UI_GETSUPPORTEDLANGUAGES
#define NSRPC2COMMUNICATION_UI_GETSUPPORTEDLANGUAGES

#include "JSONHandler/RPC2Request.h"

namespace NsRPC2Communication
{
  namespace UI
  {
    class GetSupportedLanguages : public ::NsRPC2Communication::RPC2Request
    {
    public:
        GetSupportedLanguages();
        GetSupportedLanguages(const GetSupportedLanguages&);
        virtual ~GetSupportedLanguages();

        GetSupportedLanguages& operator=(const GetSupportedLanguages&);

    private:
        friend class GetSupportedLanguagesMarshaller;
    };
  }
}

#endif