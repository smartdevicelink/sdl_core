#ifndef NSAPPLINKRPC_RESETGLOBALPROPERTIES_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_RESETGLOBALPROPERTIES_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V1/ResetGlobalProperties_response.h"


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
