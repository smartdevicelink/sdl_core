#ifndef SHOW_MARSHALLER_CLASS
#define SHOW_MARSHALLER_CLASS

#include "JSONHandler/Show.h"
#include <json/value.h>

namespace RPC2Communication
{
    class ShowMarshaller
    {
    public:
        static bool fromString(const std::string& s,Show& e);
        static const std::string toString(const Show& e);

        static bool fromJSON(const Json::Value& s,Show& e);
        static Json::Value toJSON(const Show& e);
    };
}

#endif