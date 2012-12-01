#ifndef NSAPPLINKRPC_ONHMISTATUSMARSHALLER_INCLUDE
#define NSAPPLINKRPC_ONHMISTATUSMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/OnHMIStatus.h"


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

  struct OnHMIStatusMarshaller
  {
    static bool checkIntegrity(OnHMIStatus& e);
    static bool checkIntegrityConst(const OnHMIStatus& e);
  
    static bool fromString(const std::string& s,OnHMIStatus& e);
    static const std::string toString(const OnHMIStatus& e);
  
    static bool fromJSON(const Json::Value& s,OnHMIStatus& e);
    static Json::Value toJSON(const OnHMIStatus& e);
  };
}

#endif
