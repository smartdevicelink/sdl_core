#ifndef ONBUTTONEVENTMARSHALLER_INCLUDE
#define ONBUTTONEVENTMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/OnButtonEvent.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
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
