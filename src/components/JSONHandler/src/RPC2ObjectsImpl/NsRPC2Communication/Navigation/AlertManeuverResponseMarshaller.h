#ifndef NSRPC2COMMUNICATION_NAVIGATION_ALERTMANEUVERRESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_NAVIGATION_ALERTMANEUVERRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Navigation/AlertManeuverResponse.h"

namespace NsRPC2Communication
{
  namespace Navigation
  {

    struct AlertManeuverResponseMarshaller
    {
      static bool checkIntegrity(AlertManeuverResponse& e);
      static bool checkIntegrityConst(const AlertManeuverResponse& e);
    
      static bool fromString(const std::string& s,AlertManeuverResponse& e);
      static const std::string toString(const AlertManeuverResponse& e);
    
      static bool fromJSON(const Json::Value& s,AlertManeuverResponse& e);
      static Json::Value toJSON(const AlertManeuverResponse& e);
    };
  }
}

#endif
