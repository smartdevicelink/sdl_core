#ifndef AddSubMenu_CLASS
#define AddSubMenu_CLASS

#include "JSONHandler/AddSubMenu.h"
#include <json/value.h>
#include <string>

namespace RPC2Communication
{
    class AddSubMenuMarshaller
    {
    public:
        static bool fromString(const std::string& s,AddSubMenu& e);
        static const std::string toString(const AddSubMenu& e);

        static bool fromJSON(const Json::Value& s,AddSubMenu& e);
        static Json::Value toJSON(const AddSubMenu& e);
    };
}

#endif