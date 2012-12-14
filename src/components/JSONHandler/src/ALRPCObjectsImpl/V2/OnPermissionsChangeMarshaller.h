#ifndef NSAPPLINKRPCV2_ONPERMISSIONSCHANGEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_ONPERMISSIONSCHANGEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/OnPermissionsChange.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  struct OnPermissionsChangeMarshaller
  {
    static bool checkIntegrity(OnPermissionsChange& e);
    static bool checkIntegrityConst(const OnPermissionsChange& e);
  
    static bool fromString(const std::string& s,OnPermissionsChange& e);
    static const std::string toString(const OnPermissionsChange& e);
  
    static bool fromJSON(const Json::Value& s,OnPermissionsChange& e);
    static Json::Value toJSON(const OnPermissionsChange& e);
  };
}

#endif
