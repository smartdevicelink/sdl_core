#ifndef RPC2COMMUNICATION_APPLINKCORE_ACTIVATEAPPRESPONSEMARSHALLER_INCLUDE
#define RPC2COMMUNICATION_APPLINKCORE_ACTIVATEAPPRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/RPC2Objects/AppLinkCore/ActivateAppResponse.h"

namespace RPC2Communication
{
  namespace AppLinkCore
  {

    struct ActivateAppResponseMarshaller
    {
      static bool checkIntegrity(ActivateAppResponse& e);
      static bool checkIntegrityConst(const ActivateAppResponse& e);
    
      static bool fromString(const std::string& s,ActivateAppResponse& e);
      static const std::string toString(const ActivateAppResponse& e);
    
      static bool fromJSON(const Json::Value& s,ActivateAppResponse& e);
      static Json::Value toJSON(const ActivateAppResponse& e);
    };
  }
}

#endif
