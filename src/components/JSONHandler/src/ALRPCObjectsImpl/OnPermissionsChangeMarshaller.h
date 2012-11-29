#ifndef NSAPPLINKRPC_ONPERMISSIONSCHANGEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_ONPERMISSIONSCHANGEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/OnPermissionsChange.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
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
