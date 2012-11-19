#ifndef NSRPC2COMMUNICATION_UI_SHOWRESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_SHOWRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/ShowResponse.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct ShowResponseMarshaller
    {
      static bool checkIntegrity(ShowResponse& e);
      static bool checkIntegrityConst(const ShowResponse& e);
    
      static bool fromString(const std::string& s,ShowResponse& e);
      static const std::string toString(const ShowResponse& e);
    
      static bool fromJSON(const Json::Value& s,ShowResponse& e);
      static Json::Value toJSON(const ShowResponse& e);
    };
  }
}

#endif
