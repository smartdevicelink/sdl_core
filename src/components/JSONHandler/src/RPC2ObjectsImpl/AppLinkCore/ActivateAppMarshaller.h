#ifndef RPC2COMMUNICATION_APPLINKCORE_ACTIVATEAPPMARSHALLER_INCLUDE
#define RPC2COMMUNICATION_APPLINKCORE_ACTIVATEAPPMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/RPC2Objects/AppLinkCore/ActivateApp.h"

namespace RPC2Communication
{
  namespace AppLinkCore
  {

    struct ActivateAppMarshaller
    {
      static bool checkIntegrity(ActivateApp& e);
      static bool checkIntegrityConst(const ActivateApp& e);
    
      static bool fromString(const std::string& s,ActivateApp& e);
      static const std::string toString(const ActivateApp& e);
    
      static bool fromJSON(const Json::Value& s,ActivateApp& e);
      static Json::Value toJSON(const ActivateApp& e);
    };
  }
}

#endif
