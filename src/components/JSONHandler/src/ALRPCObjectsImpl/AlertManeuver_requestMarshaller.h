#ifndef NSAPPLINKRPC_ALERTMANEUVER_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPC_ALERTMANEUVER_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/AlertManeuver_request.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

  struct AlertManeuver_requestMarshaller
  {
    static bool checkIntegrity(AlertManeuver_request& e);
    static bool checkIntegrityConst(const AlertManeuver_request& e);
  
    static bool fromString(const std::string& s,AlertManeuver_request& e);
    static const std::string toString(const AlertManeuver_request& e);
  
    static bool fromJSON(const Json::Value& s,AlertManeuver_request& e);
    static Json::Value toJSON(const AlertManeuver_request& e);
  };
}

#endif
