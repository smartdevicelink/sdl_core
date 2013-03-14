#ifndef NSRPC2COMMUNICATION_APPLINKCORE_ACTIVATEAPPRESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_ACTIVATEAPPRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/BasicCommunication/ActivateAppResponse.h"

namespace NsRPC2Communication
{
  namespace BasicCommunication
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
