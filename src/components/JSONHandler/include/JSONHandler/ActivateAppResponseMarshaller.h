#ifndef ACTIVATEAPPRESPONSE_MARSHALLER_CLASS
#define ACTIVATEAPPRESPONSE_MARSHALLER_CLASS

#include "JSONHandler/ActivateAppResponse.h"
#include <json/value.h>

namespace RPC2Communication
{
    class ActivateAppResponseMarshaller
    {
    public:
        static bool fromString(const std::string& s,ActivateAppResponse& e);
        static const std::string toString(const ActivateAppResponse& e);

        static bool fromJSON(const Json::Value& s,ActivateAppResponse& e);
        static Json::Value toJSON(const ActivateAppResponse& e);
    };
}

#endif