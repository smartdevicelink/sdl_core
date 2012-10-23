#ifndef SHOW_RESPONSE_MARSHALLER_CLASS
#define SHOW_RESPONSE_MARSHALLER_CLASS

#include <json/value.h>
#include "JSONHandler/ShowResponse.h"

namespace RPC2Communication
{
    class ShowResponseMarshaller
    {
    public:
        static bool fromString(const std::string& s,ShowResponse& e);
        static const std::string toString(const ShowResponse& e);

        static bool fromJSON(const Json::Value& s,ShowResponse& e);
        static Json::Value toJSON(const ShowResponse& e);
    };
}

#endif