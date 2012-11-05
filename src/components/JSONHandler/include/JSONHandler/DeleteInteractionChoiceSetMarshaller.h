#ifndef DELETEINTCHSETMARSHALLER
#define DELETEINTCHSETMARSHALLER

#include "JSONHandler/DeleteInteractionChoiceSet.h"
#include <json/value.h>
#include <string>

namespace RPC2Communication
{
    class DeleteInteractionChoiceSetMarshaller
    {
    public:
        static bool fromString(const std::string& s,DeleteInteractionChoiceSet& e);
        static const std::string toString(const DeleteInteractionChoiceSet& e);

        static bool fromJSON(const Json::Value& s,DeleteInteractionChoiceSet& e);
        static Json::Value toJSON(const DeleteInteractionChoiceSet& e);
    };
}

#endif