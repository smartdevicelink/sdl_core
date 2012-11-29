#ifndef NSRPC2COMMUNICATION_UI_GETDEVICELISTMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_GETDEVICELISTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/GetDeviceList.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct GetDeviceListMarshaller
    {
      static bool checkIntegrity(GetDeviceList& e);
      static bool checkIntegrityConst(const GetDeviceList& e);
    
      static bool fromString(const std::string& s,GetDeviceList& e);
      static const std::string toString(const GetDeviceList& e);
    
      static bool fromJSON(const Json::Value& s,GetDeviceList& e);
      static Json::Value toJSON(const GetDeviceList& e);
    };
  }
}

#endif
