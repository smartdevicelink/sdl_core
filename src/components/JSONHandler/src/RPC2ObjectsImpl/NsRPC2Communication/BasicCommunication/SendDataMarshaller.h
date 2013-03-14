#ifndef NSRPC2COMMUNICATION_APPLINKCORE_SENDDATAMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_SENDDATAMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/BasicCommunication/SendData.h"

namespace NsRPC2Communication
{
  namespace BasicCommunication
  {

    struct SendDataMarshaller
    {
      static bool checkIntegrity(SendData& e);
      static bool checkIntegrityConst(const SendData& e);
    
      static bool fromString(const std::string& s,SendData& e);
      static const std::string toString(const SendData& e);
    
      static bool fromJSON(const Json::Value& s,SendData& e);
      static Json::Value toJSON(const SendData& e);
    };
  }
}

#endif
