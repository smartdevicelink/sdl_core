#ifndef ONDRIVERDISTRACTIONMARSHALLER_INCLUDE
#define ONDRIVERDISTRACTIONMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>

#include "../../include/JSONHandler/ALRPCObjects/OnDriverDistraction.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


struct OnDriverDistractionMarshaller
{
  static bool checkIntegrity(OnDriverDistraction& e);
  static bool checkIntegrityConst(const OnDriverDistraction& e);

  static bool fromString(const std::string& s,OnDriverDistraction& e);
  static const std::string toString(const OnDriverDistraction& e);

  static bool fromJSON(const Json::Value& s,OnDriverDistraction& e);
  static Json::Value toJSON(const OnDriverDistraction& e);
};
#endif
