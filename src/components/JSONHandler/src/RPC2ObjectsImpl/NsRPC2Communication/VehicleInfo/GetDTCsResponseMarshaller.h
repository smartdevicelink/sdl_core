//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_VEHICLEINFO_GETDTCSRESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_VEHICLEINFO_GETDTCSRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/GetDTCsResponse.h"

namespace NsRPC2Communication
{
  namespace VehicleInfo
  {

    struct GetDTCsResponseMarshaller
    {
      static bool checkIntegrity(GetDTCsResponse& e);
      static bool checkIntegrityConst(const GetDTCsResponse& e);
    
      static bool fromString(const std::string& s,GetDTCsResponse& e);
      static const std::string toString(const GetDTCsResponse& e);
    
      static bool fromJSON(const Json::Value& s,GetDTCsResponse& e);
      static Json::Value toJSON(const GetDTCsResponse& e);
    };
  }
}

#endif
