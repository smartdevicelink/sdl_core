#ifndef NSAPPLINKRPC_PARAMETERPERMISSIONSMARSHALLER_INCLUDE
#define NSAPPLINKRPC_PARAMETERPERMISSIONSMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/ParameterPermissions.h"


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

  struct ParameterPermissionsMarshaller
  {
    static bool checkIntegrity(ParameterPermissions& e);
    static bool checkIntegrityConst(const ParameterPermissions& e);
  
    static bool fromString(const std::string& s,ParameterPermissions& e);
    static const std::string toString(const ParameterPermissions& e);
  
    static bool fromJSON(const Json::Value& s,ParameterPermissions& e);
    static Json::Value toJSON(const ParameterPermissions& e);
  };
}

#endif
