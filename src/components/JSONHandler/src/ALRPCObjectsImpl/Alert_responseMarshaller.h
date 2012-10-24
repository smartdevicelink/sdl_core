#ifndef ALERT_RESPONSEMARSHALLER_INCLUDE
#define ALERT_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/Alert_response.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct Alert_responseMarshaller
{
  static bool checkIntegrity(Alert_response& e);
  static bool checkIntegrityConst(const Alert_response& e);

  static bool fromString(const std::string& s,Alert_response& e);
  static const std::string toString(const Alert_response& e);

  static bool fromJSON(const Json::Value& s,Alert_response& e);
  static Json::Value toJSON(const Alert_response& e);
};
#endif
