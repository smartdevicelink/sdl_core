#ifndef SUBSCRIBEBUTTON_REQUESTMARSHALLER_INCLUDE
#define SUBSCRIBEBUTTON_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>

#include "../../include/JSONHandler/ALRPCObjects/SubscribeButton_request.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


struct SubscribeButton_requestMarshaller
{
  static bool checkIntegrity(SubscribeButton_request& e);
  static bool checkIntegrityConst(const SubscribeButton_request& e);

  static bool fromString(const std::string& s,SubscribeButton_request& e);
  static const std::string toString(const SubscribeButton_request& e);

  static bool fromJSON(const Json::Value& s,SubscribeButton_request& e);
  static Json::Value toJSON(const SubscribeButton_request& e);
};
#endif
