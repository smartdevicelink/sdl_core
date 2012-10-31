#ifndef ONBUTTON_PRESS_MARSHALLER_CLASS
#define ONBUTTON_PRESS_MARSHALLER_CLASS

#include "JSONHandler/OnButtonPress.h"
#include <json/value.h>
#include <string>

namespace RPC2Communication
{
    class OnButtonPressMarshaller
    {
    public:
        static bool fromString(const std::string& s,OnButtonPress& e);
        static const std::string toString(const OnButtonPress& e);

        static bool fromJSON(const Json::Value& s,OnButtonPress& e);
        static Json::Value toJSON(const OnButtonPress& e);
    };

}

#endif