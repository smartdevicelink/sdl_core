//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_SMARTDEVICELINKCORE_GETDEVICELISTRESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_SMARTDEVICELINKCORE_GETDEVICELISTRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/BasicCommunication/GetDeviceListResponse.h"

namespace NsRPC2Communication
{
  namespace BasicCommunication
  {

    struct GetDeviceListResponseMarshaller
    {
      static bool checkIntegrity(GetDeviceListResponse& e);
      static bool checkIntegrityConst(const GetDeviceListResponse& e);
    
      static bool fromString(const std::string& s,GetDeviceListResponse& e);
      static const std::string toString(const GetDeviceListResponse& e);
    
      static bool fromJSON(const Json::Value& s,GetDeviceListResponse& e);
      static Json::Value toJSON(const GetDeviceListResponse& e);
    };
  }
}

#endif
