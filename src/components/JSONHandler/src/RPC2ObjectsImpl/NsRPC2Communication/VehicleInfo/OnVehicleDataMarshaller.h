#ifndef NSRPC2COMMUNICATION_VEHICLEINFO_ONVEHICLEDATAMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_VEHICLEINFO_ONVEHICLEDATAMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/OnVehicleData.h"

namespace NsRPC2Communication
{
  namespace VehicleInfo
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
}

#endif
