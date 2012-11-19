#ifndef NSRPC2COMMUNICATION_UI_SETGLOBALPROPERTIESMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_SETGLOBALPROPERTIESMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/SetGlobalProperties.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct SetGlobalPropertiesMarshaller
    {
      static bool checkIntegrity(SetGlobalProperties& e);
      static bool checkIntegrityConst(const SetGlobalProperties& e);
    
      static bool fromString(const std::string& s,SetGlobalProperties& e);
      static const std::string toString(const SetGlobalProperties& e);
    
      static bool fromJSON(const Json::Value& s,SetGlobalProperties& e);
      static Json::Value toJSON(const SetGlobalProperties& e);
    };
  }
}

#endif
