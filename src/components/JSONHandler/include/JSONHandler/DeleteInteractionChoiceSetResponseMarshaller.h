#ifndef DELINTERCHSETRESPMARSHALLER
#define DELINTERCHSETRESPMARSHALLER

#include "JSONHandler/DeleteInteractionChoiceSetResponse.h"
#include <json/value.h>

namespace RPC2Communication
{
    class DeleteInteractionChoiceSetResponseMarshaller
    {
    public:
        static bool fromString(const std::string& s,DeleteInteractionChoiceSetResponse& e);
        static const std::string toString(const DeleteInteractionChoiceSetResponse& e);

        static bool fromJSON(const Json::Value& s,DeleteInteractionChoiceSetResponse& e);
        static Json::Value toJSON(const DeleteInteractionChoiceSetResponse& e);
    };
}

#endif