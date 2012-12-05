#ifndef NSAPPLINKRPCV2_ONHMISTATUSMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_ONHMISTATUSMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/OnHMIStatus.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
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
