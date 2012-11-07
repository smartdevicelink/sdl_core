#ifndef RPC2COMMUNICATION_UI_SETGLOBALPROPERTIESRESPONSEMARSHALLER_INCLUDE
#define RPC2COMMUNICATION_UI_SETGLOBALPROPERTIESRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/RPC2Objects/UI/SetGlobalPropertiesResponse.h"

namespace RPC2Communication
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
