#ifndef NSRPC2COMMUNICATION_VEHICLEINFO_GETVEHICLEDATARESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_VEHICLEINFO_GETVEHICLEDATARESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/GetVehicleDataResponse.h"

namespace NsRPC2Communication
{
  namespace VehicleInfo
  {

    struct GetVehicleDataResponseMarshaller
    {
      static bool checkIntegrity(GetVehicleDataResponse& e);
      static bool checkIntegrityConst(const GetVehicleDataResponse& e);
    
      static bool fromString(const std::string& s,GetVehicleDataResponse& e);
      static const std::string toString(const GetVehicleDataResponse& e);
    
      static bool fromJSON(const Json::Value& s,GetVehicleDataResponse& e);
      static Json::Value toJSON(const GetVehicleDataResponse& e);
    };
  }
}

#endif
