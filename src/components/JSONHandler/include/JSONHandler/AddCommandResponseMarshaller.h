#ifndef ADDCOMMANDRESPONSEMARSHALERR_CLASS
#define ADDCOMMANDRESPONSEMARSHALERR_CLASS

#include "JSONHandler/AddCommandResponse.h"
#include <json/value.h>
#include <string>

namespace RPC2Communication
{
    class AddCommandResponseMarshaller
    {
    public:
        static bool fromString(const std::string& s,AddCommandResponse& e);
        static const std::string toString(const AddCommandResponse& e);

        static bool fromJSON(const Json::Value& s,AddCommandResponse& e);
        static Json::Value toJSON(const AddCommandResponse& e);
    };
}

#endif