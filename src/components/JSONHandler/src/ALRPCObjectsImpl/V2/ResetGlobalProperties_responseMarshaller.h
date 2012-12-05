#ifndef NSAPPLINKRPCV2_RESETGLOBALPROPERTIES_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_RESETGLOBALPROPERTIES_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/ResetGlobalProperties_response.h"


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

  struct ResetGlobalProperties_responseMarshaller
  {
    static bool checkIntegrity(ResetGlobalProperties_response& e);
    static bool checkIntegrityConst(const ResetGlobalProperties_response& e);
  
    static bool fromString(const std::string& s,ResetGlobalProperties_response& e);
    static const std::string toString(const ResetGlobalProperties_response& e);
  
    static bool fromJSON(const Json::Value& s,ResetGlobalProperties_response& e);
    static Json::Value toJSON(const ResetGlobalProperties_response& e);
  };
}

#endif
