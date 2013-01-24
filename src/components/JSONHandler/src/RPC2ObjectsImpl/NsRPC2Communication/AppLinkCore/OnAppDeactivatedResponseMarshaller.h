#ifndef NSRPC2COMMUNICATION_APPLINKCORE_ONAPPDEACTIVATEDRESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_ONAPPDEACTIVATEDRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/OnAppDeactivatedResponse.h"

namespace NsRPC2Communication
{
  namespace AppLinkCore
  {

    struct OnAppDeactivatedResponseMarshaller
    {
      static bool checkIntegrity(OnAppDeactivatedResponse& e);
      static bool checkIntegrityConst(const OnAppDeactivatedResponse& e);
    
      static bool fromString(const std::string& s,OnAppDeactivatedResponse& e);
      static const std::string toString(const OnAppDeactivatedResponse& e);
    
      static bool fromJSON(const Json::Value& s,OnAppDeactivatedResponse& e);
      static Json::Value toJSON(const OnAppDeactivatedResponse& e);
    };
  }
}

#endif
