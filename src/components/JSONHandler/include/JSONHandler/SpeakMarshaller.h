#ifndef SPEAK_MARSHALLER_CLASS
#define SPEAK_MARSHALLER_CLASS

#include "JSONHandler/Speak.h"
#include <json/value.h>

namespace RPC2Communication
{
    class SpeakMarshaller
    {
    public:
        static bool fromString(const std::string& s,Speak& e);
        static const std::string toString(const Speak& e);

        static bool fromJSON(const Json::Value& s,Speak& e);
        static Json::Value toJSON(const Speak& e);
    };
}

#endif