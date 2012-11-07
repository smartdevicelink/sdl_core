#ifndef RPC2COMMUNICATION_UI_ONSYSTEMCONTEXTMARSHALLER_INCLUDE
#define RPC2COMMUNICATION_UI_ONSYSTEMCONTEXTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/RPC2Objects/UI/OnSystemContext.h"

namespace RPC2Communication
{
  namespace UI
  {

    struct OnSystemContextMarshaller
    {
      static bool checkIntegrity(OnSystemContext& e);
      static bool checkIntegrityConst(const OnSystemContext& e);
    
      static bool fromString(const std::string& s,OnSystemContext& e);
      static const std::string toString(const OnSystemContext& e);
    
      static bool fromJSON(const Json::Value& s,OnSystemContext& e);
      static Json::Value toJSON(const OnSystemContext& e);
    };
  }
}

#endif
