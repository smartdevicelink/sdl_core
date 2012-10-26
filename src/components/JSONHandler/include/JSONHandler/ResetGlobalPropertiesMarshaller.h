#ifndef RESETGLOBALPROPERTIES_MARSHALLER_CLASS
#define RESETGLOBALPROPERTIES_MARSHALLER_CLASS

#include "JSONHandler/ResetGlobalProperties.h"
#include <json/value.h>

namespace RPC2Communication
{
    class ResetGlobalPropertiesMarshaller
    {
    public:
        static bool fromString(const std::string& s,ResetGlobalProperties& e);
        static const std::string toString(const ResetGlobalProperties& e);

        static bool fromJSON(const Json::Value& s,ResetGlobalProperties& e);
        static Json::Value toJSON(const ResetGlobalProperties& e);
    };
}

#endif