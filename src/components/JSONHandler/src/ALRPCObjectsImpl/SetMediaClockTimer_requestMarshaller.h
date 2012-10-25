#ifndef SETMEDIACLOCKTIMER_REQUESTMARSHALLER_INCLUDE
#define SETMEDIACLOCKTIMER_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "../../include/JSONHandler/ALRPCObjects/SetMediaClockTimer_request.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


struct SetMediaClockTimer_requestMarshaller
{
  static bool checkIntegrity(SetMediaClockTimer_request& e);
  static bool checkIntegrityConst(const SetMediaClockTimer_request& e);

  static bool fromString(const std::string& s,SetMediaClockTimer_request& e);
  static const std::string toString(const SetMediaClockTimer_request& e);

  static bool fromJSON(const Json::Value& s,SetMediaClockTimer_request& e);
  static Json::Value toJSON(const SetMediaClockTimer_request& e);
};
#endif
