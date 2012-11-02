#ifndef CREATEINTERACTIONCHOICESETMARSHALLER_CLASS
#define CREATEINTERACTIONCHOICESETMARSHALLER_CLASS

#include "JSONHandler/CreateInteractionChoiceSet.h"
#include <json/value.h>
#include <string>

namespace RPC2Communication
{
    class CreateInteractionChoiceSetMarshaller
    {
    public:
        static bool fromString(const std::string& s,CreateInteractionChoiceSet& e);
        static const std::string toString(const CreateInteractionChoiceSet& e);

        static bool fromJSON(const Json::Value& s,CreateInteractionChoiceSet& e);
        static Json::Value toJSON(const CreateInteractionChoiceSet& e);
    };
}

#endif