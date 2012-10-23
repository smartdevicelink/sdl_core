#ifndef ALERT_MARSHALLER_CLASS
#define ALERT_MARSHALLER_CLASS

#include "JSONHandler/Alert.h"
#include <json/value.h>
#include <string>

namespace RPC2Communication
{
    class AlertMarshaller
    {
    public:
        static bool fromString(const std::string& s,Alert& e);
        static const std::string toString(const Alert& e);

        static bool fromJSON(const Json::Value& s,Alert& e);
        static Json::Value toJSON(const Alert& e);
    };
}

#endif