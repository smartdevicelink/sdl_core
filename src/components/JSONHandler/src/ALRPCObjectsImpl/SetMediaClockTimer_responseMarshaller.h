#ifndef SETMEDIACLOCKTIMER_RESPONSEMARSHALLER_INCLUDE
#define SETMEDIACLOCKTIMER_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/SetMediaClockTimer_response.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct SetMediaClockTimer_responseMarshaller
{
  static bool checkIntegrity(SetMediaClockTimer_response& e);
  static bool checkIntegrityConst(const SetMediaClockTimer_response& e);

  static bool fromString(const std::string& s,SetMediaClockTimer_response& e);
  static const std::string toString(const SetMediaClockTimer_response& e);

  static bool fromJSON(const Json::Value& s,SetMediaClockTimer_response& e);
  static Json::Value toJSON(const SetMediaClockTimer_response& e);
};
#endif
