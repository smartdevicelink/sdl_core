#ifndef STARTTIMEMARSHALLER_INCLUDE
#define STARTTIMEMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>

#include "../../include/JSONHandler/ALRPCObjects/StartTime.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


struct StartTimeMarshaller
{
  static bool checkIntegrity(StartTime& e);
  static bool checkIntegrityConst(const StartTime& e);

  static bool fromString(const std::string& s,StartTime& e);
  static const std::string toString(const StartTime& e);

  static bool fromJSON(const Json::Value& s,StartTime& e);
  static Json::Value toJSON(const StartTime& e);
};
#endif
