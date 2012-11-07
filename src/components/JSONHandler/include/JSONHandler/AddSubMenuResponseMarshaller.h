#ifndef ADDSUBMENURESPONSE_MARSHALLER_CLASS
#define ADDSUBMENURESPONSE_MARSHALLER_CLASS

#include "JSONHandler/AddSubMenuResponse.h"
#include <json/value.h>

namespace RPC2Communication
{
    class AddSubMenuResponseMarshaller
    {
    public:
        static bool fromString(const std::string& s,AddSubMenuResponse& e);
        static const std::string toString(const AddSubMenuResponse& e);

        static bool fromJSON(const Json::Value& s,AddSubMenuResponse& e);
        static Json::Value toJSON(const AddSubMenuResponse& e);
    };
}

#endif