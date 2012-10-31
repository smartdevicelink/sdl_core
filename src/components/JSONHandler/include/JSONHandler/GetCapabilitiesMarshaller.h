#ifndef GETCAPABILITIES_MARSHALLER_CLASS
#define GETCAPABILITIES_MARSHALLER_CLASS

#include "JSONHandler/GetCapabilities.h"
#include <json/value.h>
#include <string>

namespace RPC2Communication
{
    class GetCapabilitiesMarshaller
    {
    public:
        static bool fromString(const std::string& s,GetCapabilities& e);
        static const std::string toString(const GetCapabilities& e);

        static bool fromJSON(const Json::Value& s,GetCapabilities& e);
        static Json::Value toJSON(const GetCapabilities& e);
    };
}

#endif