#ifndef NSRPC2COMMUNICATION_APPLINKCORE_ONAPPDEACTIVATEDMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_ONAPPDEACTIVATEDMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/OnAppDeactivated.h"

namespace NsRPC2Communication
{
  namespace AppLinkCore
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
