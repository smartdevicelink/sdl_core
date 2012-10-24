#ifndef PERMISSIONGROUPITEMMARSHALLER_INCLUDE
#define PERMISSIONGROUPITEMMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/PermissionGroupItem.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct PermissionGroupItemMarshaller
{
  static bool checkIntegrity(PermissionGroupItem& e);
  static bool checkIntegrityConst(const PermissionGroupItem& e);

  static bool fromString(const std::string& s,PermissionGroupItem& e);
  static const std::string toString(const PermissionGroupItem& e);

  static bool fromJSON(const Json::Value& s,PermissionGroupItem& e);
  static Json::Value toJSON(const PermissionGroupItem& e);
};
#endif
