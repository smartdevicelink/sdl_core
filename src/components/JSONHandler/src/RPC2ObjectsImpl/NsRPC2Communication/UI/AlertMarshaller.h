#ifndef NSRPC2COMMUNICATION_UI_ALERTMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_ALERTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/Alert.h"

namespace NsRPC2Communication
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
