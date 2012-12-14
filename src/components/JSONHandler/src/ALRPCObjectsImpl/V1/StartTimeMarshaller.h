#ifndef NSAPPLINKRPC_STARTTIMEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_STARTTIMEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V1/StartTime.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Dec 13 13:37:09 2012
  source stamp	Thu Dec 13 13:33:23 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

  struct StartTimeMarshaller
  {
    static bool checkIntegrity(StartTime& e);
    static bool checkIntegrityConst(const StartTime& e);
  
    static bool fromString(const std::string& s,StartTime& e);
    static const std::string toString(const StartTime& e);
  
    static bool fromJSON(const Json::Value& s,StartTime& e);
    static Json::Value toJSON(const StartTime& e);
  };
}

#endif
