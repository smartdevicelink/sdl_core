#ifndef NSRPC2COMMUNICATION_VEHICLEINFO_UNSUBSCRIBEVEHICLEDATARESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_VEHICLEINFO_UNSUBSCRIBEVEHICLEDATARESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/UnsubscribeVehicleDataResponse.h"

namespace NsRPC2Communication
{
  namespace VehicleInfo
  {

    struct UnsubscribeVehicleDataResponseMarshaller
    {
      static bool checkIntegrity(UnsubscribeVehicleDataResponse& e);
      static bool checkIntegrityConst(const UnsubscribeVehicleDataResponse& e);
    
      static bool fromString(const std::string& s,UnsubscribeVehicleDataResponse& e);
      static const std::string toString(const UnsubscribeVehicleDataResponse& e);
    
      static bool fromJSON(const Json::Value& s,UnsubscribeVehicleDataResponse& e);
      static Json::Value toJSON(const UnsubscribeVehicleDataResponse& e);
    };
  }
}

#endif
