#ifndef ALERTMANEUVER_RESPONSEMARSHALLER_INCLUDE
#define ALERTMANEUVER_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/AlertManeuver_response.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct AlertManeuver_responseMarshaller
{
  static bool checkIntegrity(AlertManeuver_response& e);
  static bool checkIntegrityConst(const AlertManeuver_response& e);

  static bool fromString(const std::string& s,AlertManeuver_response& e);
  static const std::string toString(const AlertManeuver_response& e);

  static bool fromJSON(const Json::Value& s,AlertManeuver_response& e);
  static Json::Value toJSON(const AlertManeuver_response& e);
};
#endif
