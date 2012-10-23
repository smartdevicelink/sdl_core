#ifndef GETCAPABILITIES_RESPONSE_MARSHALLER_CLASS
#define GETCAPABILITIES_RESPONSE_MARSHALLER_CLASS

#include "JSONHandler/GetCapabilitiesResponse.h"
#include <json/value.h>
#include <string>

namespace RPC2Communication
{
    class GetCapabilitiesResponseMarshaller
    {
    public:
        static bool fromString(const std::string& s,GetCapabilitiesResponse& e);
        static const std::string toString(const GetCapabilitiesResponse& e);

        static bool fromJSON(const Json::Value& s,GetCapabilitiesResponse& e);
        static Json::Value toJSON(const GetCapabilitiesResponse& e);
    };
}

#endif