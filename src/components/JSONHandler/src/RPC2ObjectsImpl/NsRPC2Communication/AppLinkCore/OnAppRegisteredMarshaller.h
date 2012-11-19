#ifndef NSRPC2COMMUNICATION_APPLINKCORE_ONAPPREGISTEREDMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_ONAPPREGISTEREDMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/OnAppRegistered.h"

namespace NsRPC2Communication
{
  namespace AppLinkCore
  {

    struct OnAppRegisteredMarshaller
    {
      static bool checkIntegrity(OnAppRegistered& e);
      static bool checkIntegrityConst(const OnAppRegistered& e);
    
      static bool fromString(const std::string& s,OnAppRegistered& e);
      static const std::string toString(const OnAppRegistered& e);
    
      static bool fromJSON(const Json::Value& s,OnAppRegistered& e);
      static Json::Value toJSON(const OnAppRegistered& e);
    };
  }
}

#endif
