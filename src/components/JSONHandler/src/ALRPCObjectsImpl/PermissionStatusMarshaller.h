#ifndef PERMISSIONSTATUSMARSHALLER_INCLUDE
#define PERMISSIONSTATUSMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/PermissionStatus.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


//! marshalling class for PermissionStatus

class PermissionStatusMarshaller
{
public:

  static std::string toName(const PermissionStatus& e) 	{ return getName(e.mInternal) ?: ""; }

  static bool fromName(PermissionStatus& e,const std::string& s)
  { 
    return (e.mInternal=getIndex(s.c_str()))!=PermissionStatus::INVALID_ENUM;
  }

  static bool checkIntegrity(PermissionStatus& e)		{ return e.mInternal!=PermissionStatus::INVALID_ENUM; } 
  static bool checkIntegrityConst(const PermissionStatus& e)	{ return e.mInternal!=PermissionStatus::INVALID_ENUM; } 

  static bool fromString(const std::string& s,PermissionStatus& e);
  static const std::string toString(const PermissionStatus& e);

  static bool fromJSON(const Json::Value& s,PermissionStatus& e);
  static Json::Value toJSON(const PermissionStatus& e);

  static const char* getName(PermissionStatus::PermissionStatusInternal e)
  {
     return (e>=0 && e<4) ? mHashTable[e].name : NULL;
  }

  static const PermissionStatus::PermissionStatusInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[4];
};

#endif
