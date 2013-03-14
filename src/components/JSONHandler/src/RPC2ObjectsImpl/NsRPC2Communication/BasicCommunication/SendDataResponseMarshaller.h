#ifndef NSRPC2COMMUNICATION_APPLINKCORE_SENDDATARESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_SENDDATARESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/BasicCommunication/SendDataResponse.h"

namespace NsRPC2Communication
{
  namespace BasicCommunication
  {

    struct SendDataResponseMarshaller
    {
      static bool checkIntegrity(SendDataResponse& e);
      static bool checkIntegrityConst(const SendDataResponse& e);
    
      static bool fromString(const std::string& s,SendDataResponse& e);
      static const std::string toString(const SendDataResponse& e);
    
      static bool fromJSON(const Json::Value& s,SendDataResponse& e);
      static Json::Value toJSON(const SendDataResponse& e);
    };
  }
}

#endif
