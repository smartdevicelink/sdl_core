#ifndef NSRPC2COMMUNICATION_APPLINKCORE_DEACTIVATEAPPRESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_DEACTIVATEAPPRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/DeactivateAppResponse.h"

namespace NsRPC2Communication
{
  namespace AppLinkCore
  {

    struct DeactivateAppResponseMarshaller
    {
      static bool checkIntegrity(DeactivateAppResponse& e);
      static bool checkIntegrityConst(const DeactivateAppResponse& e);
    
      static bool fromString(const std::string& s,DeactivateAppResponse& e);
      static const std::string toString(const DeactivateAppResponse& e);
    
      static bool fromJSON(const Json::Value& s,DeactivateAppResponse& e);
      static Json::Value toJSON(const DeactivateAppResponse& e);
    };
  }
}

#endif
