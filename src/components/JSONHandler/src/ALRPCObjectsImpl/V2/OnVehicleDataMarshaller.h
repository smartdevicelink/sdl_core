#ifndef NSAPPLINKRPCV2_ONVEHICLEDATAMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_ONVEHICLEDATAMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/OnVehicleData.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  struct OnVehicleDataMarshaller
  {
    static bool checkIntegrity(OnVehicleData& e);
    static bool checkIntegrityConst(const OnVehicleData& e);
  
    static bool fromString(const std::string& s,OnVehicleData& e);
    static const std::string toString(const OnVehicleData& e);
  
    static bool fromJSON(const Json::Value& s,OnVehicleData& e);
    static Json::Value toJSON(const OnVehicleData& e);
  };
}

#endif
