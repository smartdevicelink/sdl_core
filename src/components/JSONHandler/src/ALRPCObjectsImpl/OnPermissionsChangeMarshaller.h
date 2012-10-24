#ifndef ONPERMISSIONSCHANGEMARSHALLER_INCLUDE
#define ONPERMISSIONSCHANGEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/OnPermissionsChange.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct OnPermissionsChangeMarshaller
{
  static bool checkIntegrity(OnPermissionsChange& e);
  static bool checkIntegrityConst(const OnPermissionsChange& e);

  static bool fromString(const std::string& s,OnPermissionsChange& e);
  static const std::string toString(const OnPermissionsChange& e);

  static bool fromJSON(const Json::Value& s,OnPermissionsChange& e);
  static Json::Value toJSON(const OnPermissionsChange& e);
};
#endif
