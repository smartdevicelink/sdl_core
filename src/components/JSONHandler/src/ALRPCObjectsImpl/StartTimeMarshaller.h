#ifndef NSAPPLINKRPC_STARTTIMEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_STARTTIMEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/StartTime.h"


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
