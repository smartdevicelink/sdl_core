#ifndef SPEECH_CAPABILITIES_MARSHALLER_CLASS
#define SPEECH_CAPABILITIES_MARSHALLER_CLASS

#include "JSONHandler/SpeechCapabilities.h"
#include <string>
#include <json/value.h>

class SpeechCapabilitiesMarshaller
{
public:
    static bool fromString(const std::string& s,SpeechCapabilities& e);
    static std::string toString(const SpeechCapabilities& e);

    static bool fromJSON(const Json::Value& s,SpeechCapabilities& e);
    static Json::Value toJSON(const SpeechCapabilities& e);

private:
    static const SpeechCapabilities::SpeechCapabilitiesInternal getIndex(const std::string &s);
    static const char* getName(SpeechCapabilities::SpeechCapabilitiesInternal e);
};

#endif