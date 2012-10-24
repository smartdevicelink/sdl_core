#ifndef VEHICLEDATARESULTMARSHALLER_INCLUDE
#define VEHICLEDATARESULTMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/VehicleDataResult.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/


struct VehicleDataResultMarshaller
{
  static bool checkIntegrity(VehicleDataResult& e);
  static bool checkIntegrityConst(const VehicleDataResult& e);

  static bool fromString(const std::string& s,VehicleDataResult& e);
  static const std::string toString(const VehicleDataResult& e);

  static bool fromJSON(const Json::Value& s,VehicleDataResult& e);
  static Json::Value toJSON(const VehicleDataResult& e);
};
#endif
