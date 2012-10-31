#ifndef ACTIVATEAPP_MARSHALLER_CLASS
#define ACTIVATEAPP_MARSHALLER_CLASS

#include "JSONHandler/ActivateApp.h"
#include <json/value.h>
#include <string>

namespace RPC2Communication
{
    class ActivateAppMarshaller
    {
    public:
        static bool fromString(const std::string& s,ActivateApp& e);
        static const std::string toString(const ActivateApp& e);

        static bool fromJSON(const Json::Value& s,ActivateApp& e);
        static Json::Value toJSON(const ActivateApp& e);
    };
}

#endif