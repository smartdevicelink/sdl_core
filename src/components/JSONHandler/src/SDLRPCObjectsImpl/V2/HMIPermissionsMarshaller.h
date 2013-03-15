//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_HMIPERMISSIONSMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_HMIPERMISSIONSMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/HMIPermissions.h"


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

  struct HMIPermissionsMarshaller
  {
    static bool checkIntegrity(HMIPermissions& e);
    static bool checkIntegrityConst(const HMIPermissions& e);
  
    static bool fromString(const std::string& s,HMIPermissions& e);
    static const std::string toString(const HMIPermissions& e);
  
    static bool fromJSON(const Json::Value& s,HMIPermissions& e);
    static Json::Value toJSON(const HMIPermissions& e);
  };
}

#endif
