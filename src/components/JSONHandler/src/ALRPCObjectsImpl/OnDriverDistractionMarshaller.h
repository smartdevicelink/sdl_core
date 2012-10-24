#ifndef ONDRIVERDISTRACTIONMARSHALLER_INCLUDE
#define ONDRIVERDISTRACTIONMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/OnDriverDistraction.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
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
