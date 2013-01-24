#ifndef NSAPPLINKRPCV2_PERMISSIONSTATUSMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_PERMISSIONSTATUSMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V2/PermissionStatus.h"


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
  
}

#endif
