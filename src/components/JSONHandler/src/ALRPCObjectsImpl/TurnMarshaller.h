#ifndef TURNMARSHALLER_INCLUDE
#define TURNMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/Turn.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/


struct TurnMarshaller
{
  static bool checkIntegrity(Turn& e);
  static bool checkIntegrityConst(const Turn& e);

  static bool fromString(const std::string& s,Turn& e);
  static const std::string toString(const Turn& e);

  static bool fromJSON(const Json::Value& s,Turn& e);
  static Json::Value toJSON(const Turn& e);
};
#endif
