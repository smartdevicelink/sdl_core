#ifndef RPC2COMMUNICATION_UI_SHOWRESPONSEMARSHALLER_INCLUDE
#define RPC2COMMUNICATION_UI_SHOWRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/RPC2Objects/UI/ShowResponse.h"

namespace RPC2Communication
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
