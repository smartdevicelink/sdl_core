#ifndef NSRPC2COMMUNICATION_UI_SHOWCONSTANTTBTMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_SHOWCONSTANTTBTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/ShowConstantTBT.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct ShowConstantTBTMarshaller
    {
      static bool checkIntegrity(ShowConstantTBT& e);
      static bool checkIntegrityConst(const ShowConstantTBT& e);
    
      static bool fromString(const std::string& s,ShowConstantTBT& e);
      static const std::string toString(const ShowConstantTBT& e);
    
      static bool fromJSON(const Json::Value& s,ShowConstantTBT& e);
      static Json::Value toJSON(const ShowConstantTBT& e);
    };
  }
}

#endif
