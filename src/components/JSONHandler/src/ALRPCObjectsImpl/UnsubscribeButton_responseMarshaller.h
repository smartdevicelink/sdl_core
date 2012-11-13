#ifndef APPLINKRPC_UNSUBSCRIBEBUTTON_RESPONSEMARSHALLER_INCLUDE
#define APPLINKRPC_UNSUBSCRIBEBUTTON_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/UnsubscribeButton_response.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Nov  7 13:10:41 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/

namespace AppLinkRPC
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
