#ifndef RESULT_MARSHALLER_CLASS
#define RESULT_MARSHALLER_CLASS

#include "JSONHandler/Result.h"
#include <json/value.h>
#include <string>

class ResultMarshaller
{
public:
    static bool fromString(const std::string& s,Result& e);
    static const std::string toString(const Result& e);

    static bool fromJSON(const Json::Value& s,Result& e);
    static Json::Value toJSON(const Result& e);
};

#endif