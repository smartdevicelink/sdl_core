#ifndef DELETECOMMAND_MARSHALLER_CLASS
#define DELETECOMMAND_MARSHALLER_CLASS

#include "JSONHandler/DeleteCommand.h"
#include <json/value.h>
#include <string>

namespace RPC2Communication
{
    class DeleteCommandMarshaller
    {
    public:
        static bool fromString(const std::string& s,DeleteCommand& e);
        static const std::string toString(const DeleteCommand& e);

        static bool fromJSON(const Json::Value& s,DeleteCommand& e);
        static Json::Value toJSON(const DeleteCommand& e);
    };

}

#endif