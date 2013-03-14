#ifndef NSAPPLINKRPCV2_SHOWCONSTANTTBT_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_SHOWCONSTANTTBT_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/ShowConstantTBT_response.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsSmartDeviceLinkRPCV2
{

  struct ShowConstantTBT_responseMarshaller
  {
    static bool checkIntegrity(ShowConstantTBT_response& e);
    static bool checkIntegrityConst(const ShowConstantTBT_response& e);
  
    static bool fromString(const std::string& s,ShowConstantTBT_response& e);
    static const std::string toString(const ShowConstantTBT_response& e);
  
    static bool fromJSON(const Json::Value& s,ShowConstantTBT_response& e);
    static Json::Value toJSON(const ShowConstantTBT_response& e);
  };
}

#endif
