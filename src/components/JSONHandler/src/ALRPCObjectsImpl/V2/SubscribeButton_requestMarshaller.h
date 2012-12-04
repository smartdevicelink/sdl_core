#ifndef NSAPPLINKRPCV2_SUBSCRIBEBUTTON_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_SUBSCRIBEBUTTON_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/SubscribeButton_request.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  struct SubscribeButton_requestMarshaller
  {
    static bool checkIntegrity(SubscribeButton_request& e);
    static bool checkIntegrityConst(const SubscribeButton_request& e);
  
    static bool fromString(const std::string& s,SubscribeButton_request& e);
    static const std::string toString(const SubscribeButton_request& e);
  
    static bool fromJSON(const Json::Value& s,SubscribeButton_request& e);
    static Json::Value toJSON(const SubscribeButton_request& e);
  };
}

#endif
