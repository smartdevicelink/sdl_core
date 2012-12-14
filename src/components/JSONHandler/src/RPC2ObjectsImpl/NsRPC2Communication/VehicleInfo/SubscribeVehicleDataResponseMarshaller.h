#ifndef NSRPC2COMMUNICATION_VEHICLEINFO_SUBSCRIBEVEHICLEDATARESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_VEHICLEINFO_SUBSCRIBEVEHICLEDATARESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/SubscribeVehicleDataResponse.h"

namespace NsRPC2Communication
{
  namespace VehicleInfo
  {

    struct SubscribeVehicleDataResponseMarshaller
    {
      static bool checkIntegrity(SubscribeVehicleDataResponse& e);
      static bool checkIntegrityConst(const SubscribeVehicleDataResponse& e);
    
      static bool fromString(const std::string& s,SubscribeVehicleDataResponse& e);
      static const std::string toString(const SubscribeVehicleDataResponse& e);
    
      static bool fromJSON(const Json::Value& s,SubscribeVehicleDataResponse& e);
      static Json::Value toJSON(const SubscribeVehicleDataResponse& e);
    };
  }
}

#endif
