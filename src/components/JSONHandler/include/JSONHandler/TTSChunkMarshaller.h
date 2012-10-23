#ifndef TTSCHUNK_MARSHALLER_CLASS
#define TTSCHUNK_MARSHALLER_CLASS

#include <string>
#include "JSONHandler/TTSChunk.h"
#include <json/value.h>

class TTSChunkMarshaller
{
public:
    static bool fromString(const std::string& s,TTSChunk& e);
    static const std::string toString(const TTSChunk& e);

    static bool fromJSON(const Json::Value& s,TTSChunk& e);
    static Json::Value toJSON(const TTSChunk& e);
};

#endif