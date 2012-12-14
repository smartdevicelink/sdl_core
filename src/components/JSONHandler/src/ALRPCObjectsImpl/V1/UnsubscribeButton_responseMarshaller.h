#ifndef NSAPPLINKRPC_UNSUBSCRIBEBUTTON_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_UNSUBSCRIBEBUTTON_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V1/UnsubscribeButton_response.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Dec 13 13:37:09 2012
  source stamp	Thu Dec 13 13:33:23 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

  struct UnsubscribeButton_responseMarshaller
  {
    static bool checkIntegrity(UnsubscribeButton_response& e);
    static bool checkIntegrityConst(const UnsubscribeButton_response& e);
  
    static bool fromString(const std::string& s,UnsubscribeButton_response& e);
    static const std::string toString(const UnsubscribeButton_response& e);
  
    static bool fromJSON(const Json::Value& s,UnsubscribeButton_response& e);
    static Json::Value toJSON(const UnsubscribeButton_response& e);
  };
}

#endif
