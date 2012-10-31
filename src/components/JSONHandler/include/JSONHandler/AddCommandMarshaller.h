#ifndef ADDCOMMANDMARSHALLER_CLASS
#define ADDCOMMANDMARSHALLER_CLASS

#include "JSONHandler/AddCommand.h"
#include <json/value.h>
#include <string>

namespace RPC2Communication
{
    class AddCommandMarshaller
    {
    public:
        static bool fromString(const std::string& s,AddCommand& e);
        static const std::string toString(const AddCommand& e);

        static bool fromJSON(const Json::Value& s,AddCommand& e);
        static Json::Value toJSON(const AddCommand& e);
    };
}

#endif