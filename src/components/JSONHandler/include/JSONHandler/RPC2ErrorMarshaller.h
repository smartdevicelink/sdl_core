#ifndef RPC2ERRORMARSHALLER_INCLUDE
#define RPC2ERRORMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "RPC2Error.h"

namespace RPC2Communication
{

  struct RPC2ErrorMarshaller
  {
    static bool fromString(const std::string& s,RPC2Error& e);
    static const std::string toString(const RPC2Error& e);

    static bool fromJSON(const Json::Value& s,RPC2Error& e);
    static Json::Value toJSON(const RPC2Error& e);
  };
}


#endif
