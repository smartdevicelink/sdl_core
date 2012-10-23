#ifndef TEXTALIGNMENT_MARSHALLER_CLASS
#define TEXTALIGNMENT_MARSHALLER_CLASS

#include "JSONHandler/TextAlignment.h"
#include <string>
#include <json/value.h>

class TextAlignmentMarshaller
{
public:
    static bool fromString(const std::string& s,TextAlignment& e);
    static std::string toString(const TextAlignment& e);

    static bool fromJSON(const Json::Value& s,TextAlignment& e);
    static Json::Value toJSON(const TextAlignment& e);

private:
    static const TextAlignment::TextAlignmentInternal getIndex(const std::string &s);
    static const char* getName(TextAlignment::TextAlignmentInternal e);
};

#endif