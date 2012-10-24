#ifndef SETGLOBALPROPERTIES_RESPONSEMARSHALLER_INCLUDE
#define SETGLOBALPROPERTIES_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/SetGlobalProperties_response.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct SetGlobalProperties_responseMarshaller
{
  static bool checkIntegrity(SetGlobalProperties_response& e);
  static bool checkIntegrityConst(const SetGlobalProperties_response& e);

  static bool fromString(const std::string& s,SetGlobalProperties_response& e);
  static const std::string toString(const SetGlobalProperties_response& e);

  static bool fromJSON(const Json::Value& s,SetGlobalProperties_response& e);
  static Json::Value toJSON(const SetGlobalProperties_response& e);
};
#endif
