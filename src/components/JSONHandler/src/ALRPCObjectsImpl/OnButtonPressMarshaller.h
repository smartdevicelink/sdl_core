#ifndef ONBUTTONPRESSMARSHALLER_INCLUDE
#define ONBUTTONPRESSMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "../../include/JSONHandler/ALRPCObjects/OnButtonPress.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
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
