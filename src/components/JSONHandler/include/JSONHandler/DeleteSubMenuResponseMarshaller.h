#ifndef DELETESUBMENURESPONSE_MARSHALLER_CLASS
#define DELETESUBMENURESPONSE_MARSHALLER_CLASS

#include "JSONHandler/DeleteSubMenuResponse.h"
#include <json/value.h>
#include <string>

namespace RPC2Communication
{
    class DeleteSubMenuResponseMarshaller
    {
    public:
        static bool fromString(const std::string& s,DeleteSubMenuResponse& e);
        static const std::string toString(const DeleteSubMenuResponse& e);

        static bool fromJSON(const Json::Value& s,DeleteSubMenuResponse& e);
        static Json::Value toJSON(const DeleteSubMenuResponse& e);
    };
}

#endif