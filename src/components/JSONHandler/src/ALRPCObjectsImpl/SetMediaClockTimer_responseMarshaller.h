#ifndef APPLINKRPC_SETMEDIACLOCKTIMER_RESPONSEMARSHALLER_INCLUDE
#define APPLINKRPC_SETMEDIACLOCKTIMER_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/SetMediaClockTimer_response.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Nov  7 13:10:41 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/

namespace AppLinkRPC
{

  struct SetMediaClockTimer_responseMarshaller
  {
    static bool checkIntegrity(SetMediaClockTimer_response& e);
    static bool checkIntegrityConst(const SetMediaClockTimer_response& e);
  
    static bool fromString(const std::string& s,SetMediaClockTimer_response& e);
    static const std::string toString(const SetMediaClockTimer_response& e);
  
    static bool fromJSON(const Json::Value& s,SetMediaClockTimer_response& e);
    static Json::Value toJSON(const SetMediaClockTimer_response& e);
  };
}

#endif
