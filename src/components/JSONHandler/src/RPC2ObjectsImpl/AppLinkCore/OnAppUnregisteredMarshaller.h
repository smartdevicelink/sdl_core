#ifndef RPC2COMMUNICATION_APPLINKCORE_ONAPPUNREGISTEREDMARSHALLER_INCLUDE
#define RPC2COMMUNICATION_APPLINKCORE_ONAPPUNREGISTEREDMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/RPC2Objects/AppLinkCore/OnAppUnregistered.h"

namespace RPC2Communication
{
  namespace AppLinkCore
  {

    struct OnAppUnregisteredMarshaller
    {
      static bool checkIntegrity(OnAppUnregistered& e);
      static bool checkIntegrityConst(const OnAppUnregistered& e);
    
      static bool fromString(const std::string& s,OnAppUnregistered& e);
      static const std::string toString(const OnAppUnregistered& e);
    
      static bool fromJSON(const Json::Value& s,OnAppUnregistered& e);
      static Json::Value toJSON(const OnAppUnregistered& e);
    };
  }
}

#endif
