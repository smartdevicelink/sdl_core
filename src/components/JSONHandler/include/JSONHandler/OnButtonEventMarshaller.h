#ifndef ONBUTTONEVENTMODE_MARSHALLER_CLASS
#define ONBUTTONEVENTMODE_MARSHALLER_CLASS

#include "JSONHandler/OnButtonEvent.h"

namespace RPC2Communication
{
    class OnButtonEventMarshaller
    {
    public:
        static bool fromString(const std::string& s,OnButtonEvent& e);
        static const std::string toString(const OnButtonEvent& e);

        static bool fromJSON(const Json::Value& s,OnButtonEvent& e);
        static Json::Value toJSON(const OnButtonEvent& e);
    };

}

#endif