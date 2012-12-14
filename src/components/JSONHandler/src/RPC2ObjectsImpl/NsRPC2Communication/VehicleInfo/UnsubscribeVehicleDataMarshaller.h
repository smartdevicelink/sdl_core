#ifndef NSRPC2COMMUNICATION_VEHICLEINFO_UNSUBSCRIBEVEHICLEDATAMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_VEHICLEINFO_UNSUBSCRIBEVEHICLEDATAMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/UnsubscribeVehicleData.h"

namespace NsRPC2Communication
{
  namespace VehicleInfo
  {

    struct UnsubscribeVehicleDataMarshaller
    {
      static bool checkIntegrity(UnsubscribeVehicleData& e);
      static bool checkIntegrityConst(const UnsubscribeVehicleData& e);
    
      static bool fromString(const std::string& s,UnsubscribeVehicleData& e);
      static const std::string toString(const UnsubscribeVehicleData& e);
    
      static bool fromJSON(const Json::Value& s,UnsubscribeVehicleData& e);
      static Json::Value toJSON(const UnsubscribeVehicleData& e);
    };
  }
}

#endif
