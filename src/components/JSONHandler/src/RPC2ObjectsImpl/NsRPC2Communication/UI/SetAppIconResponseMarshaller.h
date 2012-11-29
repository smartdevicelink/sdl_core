#ifndef NSRPC2COMMUNICATION_UI_SETAPPICONRESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_SETAPPICONRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/SetAppIconResponse.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct SetAppIconResponseMarshaller
    {
      static bool checkIntegrity(SetAppIconResponse& e);
      static bool checkIntegrityConst(const SetAppIconResponse& e);
    
      static bool fromString(const std::string& s,SetAppIconResponse& e);
      static const std::string toString(const SetAppIconResponse& e);
    
      static bool fromJSON(const Json::Value& s,SetAppIconResponse& e);
      static Json::Value toJSON(const SetAppIconResponse& e);
    };
  }
}

#endif
