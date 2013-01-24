#ifndef NSAPPLINKRPC_SUBSCRIBEBUTTON_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_SUBSCRIBEBUTTON_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V1/SubscribeButton_response.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	robok0der
*/

namespace NsAppLinkRPC
{

  struct SubscribeButton_responseMarshaller
  {
    static bool checkIntegrity(SubscribeButton_response& e);
    static bool checkIntegrityConst(const SubscribeButton_response& e);
  
    static bool fromString(const std::string& s,SubscribeButton_response& e);
    static const std::string toString(const SubscribeButton_response& e);
  
    static bool fromJSON(const Json::Value& s,SubscribeButton_response& e);
    static Json::Value toJSON(const SubscribeButton_response& e);
  };
}

#endif
