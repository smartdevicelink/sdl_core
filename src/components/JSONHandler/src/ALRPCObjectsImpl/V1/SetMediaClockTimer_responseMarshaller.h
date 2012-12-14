#ifndef NSAPPLINKRPC_SETMEDIACLOCKTIMER_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_SETMEDIACLOCKTIMER_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V1/SetMediaClockTimer_response.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Dec 14 08:11:37 2012
  source stamp	Fri Dec 14 08:11:34 2012
  author	robok0der
*/

namespace NsAppLinkRPC
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
