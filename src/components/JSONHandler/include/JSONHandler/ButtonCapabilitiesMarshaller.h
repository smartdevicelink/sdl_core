#ifndef BUTTON_CAPABILITIES_MARSHALLER
#define BUTTON_CAPABILITIES_MARSHALLER

#include "JSONHandler/ButtonCapabilities.h"
#include <json/value.h>

class ButtonCapabilitiesMarshaller
{
public:
    static bool fromString(const std::string& s,ButtonCapabilities& e);
    static const std::string toString(const ButtonCapabilities& e);

    static bool fromJSON(const Json::Value& s,ButtonCapabilities& e);
    static Json::Value toJSON(const ButtonCapabilities& e);
};

#endif