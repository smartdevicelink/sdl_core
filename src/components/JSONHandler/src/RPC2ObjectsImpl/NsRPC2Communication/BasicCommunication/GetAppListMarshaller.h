#ifndef NSRPC2COMMUNICATION_APPLINKCORE_GETAPPLISTMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_APPLINKCORE_GETAPPLISTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/BasicCommunication/GetAppList.h"

namespace NsRPC2Communication
{
  namespace BasicCommunication
  {

    struct GetAppListMarshaller
    {
      static bool checkIntegrity(GetAppList& e);
      static bool checkIntegrityConst(const GetAppList& e);
    
      static bool fromString(const std::string& s,GetAppList& e);
      static const std::string toString(const GetAppList& e);
    
      static bool fromJSON(const Json::Value& s,GetAppList& e);
      static Json::Value toJSON(const GetAppList& e);
    };
  }
}

#endif
