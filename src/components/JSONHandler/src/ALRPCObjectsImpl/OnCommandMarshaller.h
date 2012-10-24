#ifndef ONCOMMANDMARSHALLER_INCLUDE
#define ONCOMMANDMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/OnCommand.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct OnCommandMarshaller
{
  static bool checkIntegrity(OnCommand& e);
  static bool checkIntegrityConst(const OnCommand& e);

  static bool fromString(const std::string& s,OnCommand& e);
  static const std::string toString(const OnCommand& e);

  static bool fromJSON(const Json::Value& s,OnCommand& e);
  static Json::Value toJSON(const OnCommand& e);
};
#endif
