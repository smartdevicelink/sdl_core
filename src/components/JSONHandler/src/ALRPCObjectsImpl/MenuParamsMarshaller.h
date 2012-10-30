#ifndef MENUPARAMSMARSHALLER_INCLUDE
#define MENUPARAMSMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "../../include/JSONHandler/ALRPCObjects/MenuParams.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Oct 30 08:29:32 2012
  source stamp	Thu Oct 25 06:49:27 2012
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
