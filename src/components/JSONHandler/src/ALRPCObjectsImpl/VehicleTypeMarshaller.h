#ifndef VEHICLETYPEMARSHALLER_INCLUDE
#define VEHICLETYPEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/VehicleType.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct VehicleTypeMarshaller
{
  static bool checkIntegrity(VehicleType& e);
  static bool checkIntegrityConst(const VehicleType& e);

  static bool fromString(const std::string& s,VehicleType& e);
  static const std::string toString(const VehicleType& e);

  static bool fromJSON(const Json::Value& s,VehicleType& e);
  static Json::Value toJSON(const VehicleType& e);
};
#endif
