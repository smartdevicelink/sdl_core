#ifndef RPC2COMMUNICATION_UI_ONAPPDEACTIVATEDMARSHALLER_INCLUDE
#define RPC2COMMUNICATION_UI_ONAPPDEACTIVATEDMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/RPC2Objects/UI/OnAppDeactivated.h"

namespace RPC2Communication
{
  namespace UI
  {

    struct OnAppDeactivatedMarshaller
    {
      static bool checkIntegrity(OnAppDeactivated& e);
      static bool checkIntegrityConst(const OnAppDeactivated& e);
    
      static bool fromString(const std::string& s,OnAppDeactivated& e);
      static const std::string toString(const OnAppDeactivated& e);
    
      static bool fromJSON(const Json::Value& s,OnAppDeactivated& e);
      static Json::Value toJSON(const OnAppDeactivated& e);
    };
  }
}

#endif
