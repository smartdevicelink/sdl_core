#ifndef BUTTON_EVENT_MODE_MARSHALLER
#define BUTTON_EVENT_MODE_MARSHALLER

#include "JSONHandler/ButtonEventMode.h"
#include <string>
#include <json/value.h>

class ButtonEventModeMarshaller
{
public:
    static bool fromString(const std::string& s,ButtonEventMode& e);
    static std::string toString(const ButtonEventMode& e);

    static bool fromJSON(const Json::Value& s,ButtonEventMode& e);
    static Json::Value toJSON(const ButtonEventMode& e);

};

#endif