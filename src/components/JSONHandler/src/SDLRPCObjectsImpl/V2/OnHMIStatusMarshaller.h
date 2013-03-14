#ifndef NSAPPLINKRPCV2_ONHMISTATUSMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_ONHMISTATUSMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/OnHMIStatus.h"


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

  struct OnHMIStatusMarshaller
  {
    static bool checkIntegrity(OnHMIStatus& e);
    static bool checkIntegrityConst(const OnHMIStatus& e);
  
    static bool fromString(const std::string& s,OnHMIStatus& e);
    static const std::string toString(const OnHMIStatus& e);
  
    static bool fromJSON(const Json::Value& s,OnHMIStatus& e);
    static Json::Value toJSON(const OnHMIStatus& e);
  };
}

#endif
