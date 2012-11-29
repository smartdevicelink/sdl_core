#ifndef NSRPC2COMMUNICATION_VEHICLEINFO_GETVEHICLETYPEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_VEHICLEINFO_GETVEHICLETYPEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/GetVehicleType.h"

namespace NsRPC2Communication
{
  namespace VehicleInfo
  {

    struct GetVehicleTypeMarshaller
    {
      static bool checkIntegrity(GetVehicleType& e);
      static bool checkIntegrityConst(const GetVehicleType& e);
    
      static bool fromString(const std::string& s,GetVehicleType& e);
      static const std::string toString(const GetVehicleType& e);
    
      static bool fromJSON(const Json::Value& s,GetVehicleType& e);
      static Json::Value toJSON(const GetVehicleType& e);
    };
  }
}

#endif
