#ifndef NSRPC2COMMUNICATION_NAVIGATION_ALERTMANEUVERMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_NAVIGATION_ALERTMANEUVERMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Navigation/AlertManeuver.h"

namespace NsRPC2Communication
{
  namespace Navigation
  {

    struct AlertManeuverMarshaller
    {
      static bool checkIntegrity(AlertManeuver& e);
      static bool checkIntegrityConst(const AlertManeuver& e);
    
      static bool fromString(const std::string& s,AlertManeuver& e);
      static const std::string toString(const AlertManeuver& e);
    
      static bool fromJSON(const Json::Value& s,AlertManeuver& e);
      static Json::Value toJSON(const AlertManeuver& e);
    };
  }
}

#endif
