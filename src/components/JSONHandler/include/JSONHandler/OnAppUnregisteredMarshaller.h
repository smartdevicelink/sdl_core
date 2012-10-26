#ifndef ONAPPUNREGISTERED_MARSHALLER_CLASS
#define ONAPPUNREGISTERED_MARSHALLER_CLASS

#include "JSONHandler/OnAppUnregistered.h"

namespace RPC2Communication
{
    class OnAppUnregisteredMarshaller
    {
    public:
        static bool fromString(const std::string& s,OnAppUnregistered& e);
        static const std::string toString(const OnAppUnregistered& e);

        static bool fromJSON(const Json::Value& s,OnAppUnregistered& e);
        static Json::Value toJSON(const OnAppUnregistered& e);
    };
}

#endif