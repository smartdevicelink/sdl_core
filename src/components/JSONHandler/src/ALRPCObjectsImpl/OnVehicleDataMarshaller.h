#ifndef ONVEHICLEDATAMARSHALLER_INCLUDE
#define ONVEHICLEDATAMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/OnVehicleData.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct OnVehicleDataMarshaller
{
  static bool checkIntegrity(OnVehicleData& e);
  static bool checkIntegrityConst(const OnVehicleData& e);

  static bool fromString(const std::string& s,OnVehicleData& e);
  static const std::string toString(const OnVehicleData& e);

  static bool fromJSON(const Json::Value& s,OnVehicleData& e);
  static Json::Value toJSON(const OnVehicleData& e);
};
#endif
