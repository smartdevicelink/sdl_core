#ifndef DELETESUBMENU_MARSHALLER_CLASS
#define DELETESUBMENU_MARSHALLER_CLASS

#include "JSONHandler/DeleteSubMenu.h"
#include <json/value.h>
#include <string>

namespace RPC2Communication
{
    class DeleteSubMenuMarshaller
    {
    public:
        static bool fromString(const std::string& s,DeleteSubMenu& e);
        static const std::string toString(const DeleteSubMenu& e);

        static bool fromJSON(const Json::Value& s,DeleteSubMenu& e);
        static Json::Value toJSON(const DeleteSubMenu& e);
    };
}

#endif