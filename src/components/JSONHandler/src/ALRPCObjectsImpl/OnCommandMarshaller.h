#ifndef ONCOMMANDMARSHALLER_INCLUDE
#define ONCOMMANDMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>

#include "../../include/JSONHandler/ALRPCObjects/OnCommand.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
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
