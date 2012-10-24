#ifndef GETVEHICLEDATA_RESPONSEMARSHALLER_INCLUDE
#define GETVEHICLEDATA_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/GetVehicleData_response.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct GetVehicleData_responseMarshaller
{
  static bool checkIntegrity(GetVehicleData_response& e);
  static bool checkIntegrityConst(const GetVehicleData_response& e);

  static bool fromString(const std::string& s,GetVehicleData_response& e);
  static const std::string toString(const GetVehicleData_response& e);

  static bool fromJSON(const Json::Value& s,GetVehicleData_response& e);
  static Json::Value toJSON(const GetVehicleData_response& e);
};
#endif
