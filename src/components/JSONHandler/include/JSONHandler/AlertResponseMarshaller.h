#ifndef ALERT_RESPONSE_MARSHALLER_CLASS
#define ALERT_RESPONSE_MARSHALLER_CLASS

#include "JSONHandler/AlertResponse.h"
#include <json/value.h>

namespace RPC2Communication
{
    class AlertResponseMarshaller
    {
    public:
        static bool fromString(const std::string& s,AlertResponse& e);
        static const std::string toString(const AlertResponse& e);

        static bool fromJSON(const Json::Value& s,AlertResponse& e);
        static Json::Value toJSON(const AlertResponse& e);
    };
}

#endif