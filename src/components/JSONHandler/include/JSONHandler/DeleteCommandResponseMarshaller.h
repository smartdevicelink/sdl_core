#ifndef DELETECOMMANDRESPONSE_MARSHALLER_CLASS
#define DELETECOMMANDRESPONSE_MARSHALLER_CLASS

#include "JSONHandler/DeleteCommandResponse.h"
#include <json/value.h>

namespace RPC2Communication
{
    class DeleteCommandResponseMarshaller
    {
    public:
        static bool fromString(const std::string& s,DeleteCommandResponse& e);
        static const std::string toString(const DeleteCommandResponse& e);

        static bool fromJSON(const Json::Value& s,DeleteCommandResponse& e);
        static Json::Value toJSON(const DeleteCommandResponse& e);
    };
}


#endif