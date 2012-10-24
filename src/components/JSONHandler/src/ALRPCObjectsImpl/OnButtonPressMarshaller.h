#ifndef ONBUTTONPRESSMARSHALLER_INCLUDE
#define ONBUTTONPRESSMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>

#include "../../include/JSONHandler/ALRPCObjects/OnButtonPress.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


struct OnButtonPressMarshaller
{
  static bool checkIntegrity(OnButtonPress& e);
  static bool checkIntegrityConst(const OnButtonPress& e);

  static bool fromString(const std::string& s,OnButtonPress& e);
  static const std::string toString(const OnButtonPress& e);

  static bool fromJSON(const Json::Value& s,OnButtonPress& e);
  static Json::Value toJSON(const OnButtonPress& e);
};
#endif
