//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_HMIAPPLICATIONMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_HMIAPPLICATIONMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/HMIApplication.h"


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

  struct HMIApplicationMarshaller
  {
    static bool checkIntegrity(HMIApplication& e);
    static bool checkIntegrityConst(const HMIApplication& e);
  
    static bool fromString(const std::string& s,HMIApplication& e);
    static const std::string toString(const HMIApplication& e);
  
    static bool fromJSON(const Json::Value& s,HMIApplication& e);
    static Json::Value toJSON(const HMIApplication& e);
  };
}

#endif
