#ifndef NSRPC2COMMUNICATION_VEHICLEINFO_GETVEHICLEDATAMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_VEHICLEINFO_GETVEHICLEDATAMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/GetVehicleData.h"

namespace NsRPC2Communication
{
  namespace VehicleInfo
  {

    struct GetVehicleDataMarshaller
    {
      static bool checkIntegrity(GetVehicleData& e);
      static bool checkIntegrityConst(const GetVehicleData& e);
    
      static bool fromString(const std::string& s,GetVehicleData& e);
      static const std::string toString(const GetVehicleData& e);
    
      static bool fromJSON(const Json::Value& s,GetVehicleData& e);
      static Json::Value toJSON(const GetVehicleData& e);
    };
  }
}

#endif
