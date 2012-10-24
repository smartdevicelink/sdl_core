#ifndef STARTTIMEMARSHALLER_INCLUDE
#define STARTTIMEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/StartTime.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
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
