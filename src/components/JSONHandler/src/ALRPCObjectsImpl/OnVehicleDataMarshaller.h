#ifndef NSAPPLINKRPC_ONVEHICLEDATAMARSHALLER_INCLUDE
#define NSAPPLINKRPC_ONVEHICLEDATAMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/OnVehicleData.h"


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
