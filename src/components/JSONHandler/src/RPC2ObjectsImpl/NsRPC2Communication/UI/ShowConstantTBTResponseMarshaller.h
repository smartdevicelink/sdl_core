#ifndef NSRPC2COMMUNICATION_UI_SHOWCONSTANTTBTRESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_SHOWCONSTANTTBTRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/ShowConstantTBTResponse.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct ShowConstantTBTResponseMarshaller
    {
      static bool checkIntegrity(ShowConstantTBTResponse& e);
      static bool checkIntegrityConst(const ShowConstantTBTResponse& e);
    
      static bool fromString(const std::string& s,ShowConstantTBTResponse& e);
      static const std::string toString(const ShowConstantTBTResponse& e);
    
      static bool fromJSON(const Json::Value& s,ShowConstantTBTResponse& e);
      static Json::Value toJSON(const ShowConstantTBTResponse& e);
    };
  }
}

#endif
