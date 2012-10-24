#ifndef MENUPARAMSMARSHALLER_INCLUDE
#define MENUPARAMSMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/MenuParams.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct MenuParamsMarshaller
{
  static bool checkIntegrity(MenuParams& e);
  static bool checkIntegrityConst(const MenuParams& e);

  static bool fromString(const std::string& s,MenuParams& e);
  static const std::string toString(const MenuParams& e);

  static bool fromJSON(const Json::Value& s,MenuParams& e);
  static Json::Value toJSON(const MenuParams& e);
};
#endif
