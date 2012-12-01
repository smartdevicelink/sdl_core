#ifndef NSRPC2COMMUNICATION_VEHICLEINFO_GETVEHICLETYPERESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_VEHICLEINFO_GETVEHICLETYPERESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/GetVehicleTypeResponse.h"

namespace NsRPC2Communication
{
  namespace VehicleInfo
  {

    struct GetVehicleTypeResponseMarshaller
    {
      static bool checkIntegrity(GetVehicleTypeResponse& e);
      static bool checkIntegrityConst(const GetVehicleTypeResponse& e);
    
      static bool fromString(const std::string& s,GetVehicleTypeResponse& e);
      static const std::string toString(const GetVehicleTypeResponse& e);
    
      static bool fromJSON(const Json::Value& s,GetVehicleTypeResponse& e);
      static Json::Value toJSON(const GetVehicleTypeResponse& e);
    };
  }
}

#endif
