#ifndef RPC2COMMUNICATION_UI_ALERTMARSHALLER_INCLUDE
#define RPC2COMMUNICATION_UI_ALERTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/RPC2Objects/UI/Alert.h"

namespace RPC2Communication
{
  namespace UI
  {

    struct AlertMarshaller
    {
      static bool checkIntegrity(Alert& e);
      static bool checkIntegrityConst(const Alert& e);
    
      static bool fromString(const std::string& s,Alert& e);
      static const std::string toString(const Alert& e);
    
      static bool fromJSON(const Json::Value& s,Alert& e);
      static Json::Value toJSON(const Alert& e);
    };
  }
}

#endif
