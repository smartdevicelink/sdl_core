#ifndef DIALNUMBER_REQUESTMARSHALLER_INCLUDE
#define DIALNUMBER_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/DialNumber_request.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct DialNumber_requestMarshaller
{
  static bool checkIntegrity(DialNumber_request& e);
  static bool checkIntegrityConst(const DialNumber_request& e);

  static bool fromString(const std::string& s,DialNumber_request& e);
  static const std::string toString(const DialNumber_request& e);

  static bool fromJSON(const Json::Value& s,DialNumber_request& e);
  static Json::Value toJSON(const DialNumber_request& e);
};
#endif
