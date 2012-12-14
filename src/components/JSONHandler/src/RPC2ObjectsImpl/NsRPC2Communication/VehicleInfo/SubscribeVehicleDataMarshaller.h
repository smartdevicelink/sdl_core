#ifndef NSRPC2COMMUNICATION_VEHICLEINFO_SUBSCRIBEVEHICLEDATAMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_VEHICLEINFO_SUBSCRIBEVEHICLEDATAMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/SubscribeVehicleData.h"

namespace NsRPC2Communication
{
  namespace VehicleInfo
  {

    struct SubscribeVehicleDataMarshaller
    {
      static bool checkIntegrity(SubscribeVehicleData& e);
      static bool checkIntegrityConst(const SubscribeVehicleData& e);
    
      static bool fromString(const std::string& s,SubscribeVehicleData& e);
      static const std::string toString(const SubscribeVehicleData& e);
    
      static bool fromJSON(const Json::Value& s,SubscribeVehicleData& e);
      static Json::Value toJSON(const SubscribeVehicleData& e);
    };
  }
}

#endif
