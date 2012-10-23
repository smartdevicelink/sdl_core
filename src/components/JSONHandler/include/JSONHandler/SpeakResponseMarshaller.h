#ifndef SPEAKRESPONSE_MARSHALLER_CLASS
#define SPEAKRESPONSE_MARSHALLER_CLASS

#include "JSONHandler/SpeakResponse.h"
#include <json/value.h>

namespace RPC2Communication
{
    class SpeakResponseMarshaller
    {
    public:
        static bool fromString(const std::string& s,SpeakResponse& e);
        static const std::string toString(const SpeakResponse& e);

        static bool fromJSON(const Json::Value& s,SpeakResponse& e);
        static Json::Value toJSON(const SpeakResponse& e);
    };
}

#endif