#ifndef SETMEDIACLOCKTIMER_RESPONSEMARSHALLER_INCLUDE
#define SETMEDIACLOCKTIMER_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "../../include/JSONHandler/ALRPCObjects/SetMediaClockTimer_response.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
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
