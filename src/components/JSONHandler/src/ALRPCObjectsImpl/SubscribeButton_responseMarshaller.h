#ifndef SUBSCRIBEBUTTON_RESPONSEMARSHALLER_INCLUDE
#define SUBSCRIBEBUTTON_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>

#include "../../include/JSONHandler/ALRPCObjects/SubscribeButton_response.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


struct SubscribeButton_responseMarshaller
{
  static bool checkIntegrity(SubscribeButton_response& e);
  static bool checkIntegrityConst(const SubscribeButton_response& e);

  static bool fromString(const std::string& s,SubscribeButton_response& e);
  static const std::string toString(const SubscribeButton_response& e);

  static bool fromJSON(const Json::Value& s,SubscribeButton_response& e);
  static Json::Value toJSON(const SubscribeButton_response& e);
};
#endif
