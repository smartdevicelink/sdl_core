#ifndef UNSUBSCRIBEBUTTON_RESPONSEMARSHALLER_INCLUDE
#define UNSUBSCRIBEBUTTON_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/UnsubscribeButton_response.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct UnsubscribeButton_responseMarshaller
{
  static bool checkIntegrity(UnsubscribeButton_response& e);
  static bool checkIntegrityConst(const UnsubscribeButton_response& e);

  static bool fromString(const std::string& s,UnsubscribeButton_response& e);
  static const std::string toString(const UnsubscribeButton_response& e);

  static bool fromJSON(const Json::Value& s,UnsubscribeButton_response& e);
  static Json::Value toJSON(const UnsubscribeButton_response& e);
};
#endif
