//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_RESETGLOBALPROPERTIES_RESPONSEMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_RESETGLOBALPROPERTIES_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/ResetGlobalProperties_response.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPCV2
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
