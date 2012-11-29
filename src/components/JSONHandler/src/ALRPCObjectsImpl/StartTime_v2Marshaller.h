#ifndef NSAPPLINKRPC_STARTTIME_V2MARSHALLER_INCLUDE
#define NSAPPLINKRPC_STARTTIME_V2MARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/StartTime_v2.h"


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

  struct StartTime_v2Marshaller
  {
    static bool checkIntegrity(StartTime_v2& e);
    static bool checkIntegrityConst(const StartTime_v2& e);
  
    static bool fromString(const std::string& s,StartTime_v2& e);
    static const std::string toString(const StartTime_v2& e);
  
    static bool fromJSON(const Json::Value& s,StartTime_v2& e);
    static Json::Value toJSON(const StartTime_v2& e);
  };
}

#endif
