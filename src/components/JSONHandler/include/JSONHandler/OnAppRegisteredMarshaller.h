#ifndef ONAPPREGISTERED_MARSHALLER_CLASS
#define ONAPPREGISTERED_MARSHALLER_CLASS

#include "JSONHandler/OnAppRegistered.h"

namespace RPC2Communication
{
    class OnAppRegisteredMarshaller
    {
    public:
        static bool fromString(const std::string& s,OnAppRegistered& e);
        static const std::string toString(const OnAppRegistered& e);

        static bool fromJSON(const Json::Value& s,OnAppRegistered& e);
        static Json::Value toJSON(const OnAppRegistered& e);
    };
}

#endif