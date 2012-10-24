#ifndef ONBUTTONEVENTMARSHALLER_INCLUDE
#define ONBUTTONEVENTMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>

#include "../../include/JSONHandler/ALRPCObjects/OnButtonEvent.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


struct OnButtonEventMarshaller
{
  static bool checkIntegrity(OnButtonEvent& e);
  static bool checkIntegrityConst(const OnButtonEvent& e);

  static bool fromString(const std::string& s,OnButtonEvent& e);
  static const std::string toString(const OnButtonEvent& e);

  static bool fromJSON(const Json::Value& s,OnButtonEvent& e);
  static Json::Value toJSON(const OnButtonEvent& e);
};
#endif
