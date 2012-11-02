#ifndef CICSMARSHALLER_CLASS
#define CICSMARSHALLER_CLASS

#include "JSONHandler/CreateInteractionChoiceSetResponse.h"
#include <json/value.h>

namespace RPC2Communication
{
    class CreateInteractionChoiceSetResponseMarshaller
    {
    public:
        static bool fromString(const std::string& s,CreateInteractionChoiceSetResponse& e);
        static const std::string toString(const CreateInteractionChoiceSetResponse& e);

        static bool fromJSON(const Json::Value& s,CreateInteractionChoiceSetResponse& e);
        static Json::Value toJSON(const CreateInteractionChoiceSetResponse& e);
    };
}

#endif