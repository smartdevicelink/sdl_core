#ifndef UIONCOMMANDMARSHALLER_CLASS
#define UIONCOMMANDMARSHALLER_CLASS

#include "JSONHandler/OnCommand.h"
#include <json/value.h>
#include <string>

namespace RPC2Communication
{
    class OnCommandMarshaller
    {
    public:
        static bool fromString(const std::string& s,OnCommand& e);
        static const std::string toString(const OnCommand& e);

        static bool fromJSON(const Json::Value& s,OnCommand& e);
        static Json::Value toJSON(const OnCommand& e);
    };
}

#endif