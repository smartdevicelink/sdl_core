#ifndef PERFORMINTERACTIONMARSHALLER
#define PERFORMINTERACTIONMARSHALLER

#include "JSONHandler/PerformInteraction.h"
#include <json/value.h>
#include <string>

namespace RPC2Communication
{
    class PerformInteractionMarshaller 
    {
    public:
        static bool fromString(const std::string& s,PerformInteraction& e);
        static const std::string toString(const PerformInteraction& e);

        static bool fromJSON(const Json::Value& s,PerformInteraction& e);
        static Json::Value toJSON(const PerformInteraction& e);
    };
}

#endif