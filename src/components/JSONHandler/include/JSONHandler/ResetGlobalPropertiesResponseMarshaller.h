#ifndef RESETGP_RESPONSE_MARSHALLER_CLASS
#define RESETGP_RESPONSE_MARSHALLER_CLASS

#include "JSONHandler/ResetGlobalPropertiesResponse.h"
#include <json/value.h>
#include <string>

namespace RPC2Communication
{
    class ResetGlobalPropertiesResponseMarshaller
    {
    public:
        static bool fromString(const std::string& s,ResetGlobalPropertiesResponse& e);
        static const std::string & toString(const ResetGlobalPropertiesResponse& e);

        static bool fromJSON(const Json::Value& s,ResetGlobalPropertiesResponse& e);
        static Json::Value toJSON(const ResetGlobalPropertiesResponse& e);
    };
}

#endif