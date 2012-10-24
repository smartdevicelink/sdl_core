#ifndef SETGLOBALPROPERTIESRESPONSE_MARSHALLER_CLASS
#define SETGLOBALPROPERTIESRESPONSE_MARSHALLER_CLASS

#include "JSONHandler/SetGlobalPropertiesResponse.h"
#include <json/value.h>

namespace RPC2Communication
{
    class SetGlobalPropertiesResponseMarshaller
    {
    public:
        static bool fromString(const std::string& s,SetGlobalPropertiesResponse& e);
        static const std::string toString(const SetGlobalPropertiesResponse& e);

        static bool fromJSON(const Json::Value& s,SetGlobalPropertiesResponse& e);
        static Json::Value toJSON(const SetGlobalPropertiesResponse& e);
    };
}

#endif