#ifndef ONLANGUAGECHANGEMARSHALLER_INCLUDE
#define ONLANGUAGECHANGEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/OnLanguageChange.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct OnLanguageChangeMarshaller
{
  static bool checkIntegrity(OnLanguageChange& e);
  static bool checkIntegrityConst(const OnLanguageChange& e);

  static bool fromString(const std::string& s,OnLanguageChange& e);
  static const std::string toString(const OnLanguageChange& e);

  static bool fromJSON(const Json::Value& s,OnLanguageChange& e);
  static Json::Value toJSON(const OnLanguageChange& e);
};
#endif
