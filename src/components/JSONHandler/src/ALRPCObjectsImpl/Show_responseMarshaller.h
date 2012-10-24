#ifndef SHOW_RESPONSEMARSHALLER_INCLUDE
#define SHOW_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>

#include "../../include/JSONHandler/ALRPCObjects/Show_response.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


struct Show_responseMarshaller
{
  static bool checkIntegrity(Show_response& e);
  static bool checkIntegrityConst(const Show_response& e);

  static bool fromString(const std::string& s,Show_response& e);
  static const std::string toString(const Show_response& e);

  static bool fromJSON(const Json::Value& s,Show_response& e);
  static Json::Value toJSON(const Show_response& e);
};
#endif
