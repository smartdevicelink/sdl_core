#ifndef SETGLOBALPROPERTIES_MARSHALLER_CLASS
#define SETGLOBALPROPERTIES_MARSHALLER_CLASS

#include "JSONHandler/SetGlobalProperties.h"
#include <json/value.h>
#include <string>

namespace RPC2Communication
{
    class SetGlobalPropertiesMarshaller
    {
    public:
        static bool fromString(const std::string& s,SetGlobalProperties& e);
        static const std::string toString(const SetGlobalProperties& e);

        static bool fromJSON(const Json::Value& s,SetGlobalProperties& e);
        static Json::Value toJSON(const SetGlobalProperties& e);
    };
}

#endif