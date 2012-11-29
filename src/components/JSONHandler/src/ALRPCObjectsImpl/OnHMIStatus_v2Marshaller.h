#ifndef NSAPPLINKRPC_ONHMISTATUS_V2MARSHALLER_INCLUDE
#define NSAPPLINKRPC_ONHMISTATUS_V2MARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/OnHMIStatus_v2.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

  struct OnHMIStatus_v2Marshaller
  {
    static bool checkIntegrity(OnHMIStatus_v2& e);
    static bool checkIntegrityConst(const OnHMIStatus_v2& e);
  
    static bool fromString(const std::string& s,OnHMIStatus_v2& e);
    static const std::string toString(const OnHMIStatus_v2& e);
  
    static bool fromJSON(const Json::Value& s,OnHMIStatus_v2& e);
    static Json::Value toJSON(const OnHMIStatus_v2& e);
  };
}

#endif
