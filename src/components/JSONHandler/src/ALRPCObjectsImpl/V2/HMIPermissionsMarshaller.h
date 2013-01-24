#ifndef NSAPPLINKRPCV2_HMIPERMISSIONSMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_HMIPERMISSIONSMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/HMIPermissions.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsAppLinkRPCV2
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
