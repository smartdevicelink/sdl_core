#ifndef NSRPC2COMMUNICATION_APPLINKCORE_DEACTIVATEAPPMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_DEACTIVATEAPPMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/DeactivateApp.h"

namespace NsRPC2Communication
{
  namespace AppLinkCore
  {

    struct DeactivateAppMarshaller
    {
      static bool checkIntegrity(DeactivateApp& e);
      static bool checkIntegrityConst(const DeactivateApp& e);
    
      static bool fromString(const std::string& s,DeactivateApp& e);
      static const std::string toString(const DeactivateApp& e);
    
      static bool fromJSON(const Json::Value& s,DeactivateApp& e);
      static Json::Value toJSON(const DeactivateApp& e);
    };
  }
}

#endif
