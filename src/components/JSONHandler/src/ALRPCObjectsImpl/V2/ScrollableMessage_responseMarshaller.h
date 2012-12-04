#ifndef NSAPPLINKRPCV2_SCROLLABLEMESSAGE_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_SCROLLABLEMESSAGE_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/ScrollableMessage_response.h"


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

  struct ScrollableMessage_responseMarshaller
  {
    static bool checkIntegrity(ScrollableMessage_response& e);
    static bool checkIntegrityConst(const ScrollableMessage_response& e);
  
    static bool fromString(const std::string& s,ScrollableMessage_response& e);
    static const std::string toString(const ScrollableMessage_response& e);
  
    static bool fromJSON(const Json::Value& s,ScrollableMessage_response& e);
    static Json::Value toJSON(const ScrollableMessage_response& e);
  };
}

#endif
