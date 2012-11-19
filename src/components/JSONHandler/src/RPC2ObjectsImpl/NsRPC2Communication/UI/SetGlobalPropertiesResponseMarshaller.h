#ifndef NSRPC2COMMUNICATION_UI_SETGLOBALPROPERTIESRESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_SETGLOBALPROPERTIESRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/SetGlobalPropertiesResponse.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct SetGlobalPropertiesResponseMarshaller
    {
      static bool checkIntegrity(SetGlobalPropertiesResponse& e);
      static bool checkIntegrityConst(const SetGlobalPropertiesResponse& e);
    
      static bool fromString(const std::string& s,SetGlobalPropertiesResponse& e);
      static const std::string toString(const SetGlobalPropertiesResponse& e);
    
      static bool fromJSON(const Json::Value& s,SetGlobalPropertiesResponse& e);
      static Json::Value toJSON(const SetGlobalPropertiesResponse& e);
    };
  }
}

#endif
