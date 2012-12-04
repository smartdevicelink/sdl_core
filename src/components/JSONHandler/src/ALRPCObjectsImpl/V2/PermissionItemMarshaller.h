#ifndef NSAPPLINKRPCV2_PERMISSIONITEMMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_PERMISSIONITEMMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/PermissionItem.h"


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

  struct PermissionItemMarshaller
  {
    static bool checkIntegrity(PermissionItem& e);
    static bool checkIntegrityConst(const PermissionItem& e);
  
    static bool fromString(const std::string& s,PermissionItem& e);
    static const std::string toString(const PermissionItem& e);
  
    static bool fromJSON(const Json::Value& s,PermissionItem& e);
    static Json::Value toJSON(const PermissionItem& e);
  };
}

#endif
