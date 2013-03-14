#ifndef NSRPC2COMMUNICATION_APPLINKCORE_GETAPPLISTRESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_GETAPPLISTRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/BasicCommunication/GetAppListResponse.h"

namespace NsRPC2Communication
{
  namespace BasicCommunication
  {

    struct GetAppListResponseMarshaller
    {
      static bool checkIntegrity(GetAppListResponse& e);
      static bool checkIntegrityConst(const GetAppListResponse& e);
    
      static bool fromString(const std::string& s,GetAppListResponse& e);
      static const std::string toString(const GetAppListResponse& e);
    
      static bool fromJSON(const Json::Value& s,GetAppListResponse& e);
      static Json::Value toJSON(const GetAppListResponse& e);
    };
  }
}

#endif
