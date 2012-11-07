#ifndef RPC2COMMUNICATION_UI_RESETGLOBALPROPERTIESMARSHALLER_INCLUDE
#define RPC2COMMUNICATION_UI_RESETGLOBALPROPERTIESMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/RPC2Objects/UI/ResetGlobalProperties.h"

namespace RPC2Communication
{
  namespace UI
  {

    struct ResetGlobalPropertiesMarshaller
    {
      static bool checkIntegrity(ResetGlobalProperties& e);
      static bool checkIntegrityConst(const ResetGlobalProperties& e);
    
      static bool fromString(const std::string& s,ResetGlobalProperties& e);
      static const std::string toString(const ResetGlobalProperties& e);
    
      static bool fromJSON(const Json::Value& s,ResetGlobalProperties& e);
      static Json::Value toJSON(const ResetGlobalProperties& e);
    };
  }
}

#endif
