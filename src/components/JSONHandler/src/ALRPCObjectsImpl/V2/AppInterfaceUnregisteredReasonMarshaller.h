#ifndef NSAPPLINKRPCV2_APPINTERFACEUNREGISTEREDREASONMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_APPINTERFACEUNREGISTEREDREASONMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V2/AppInterfaceUnregisteredReason.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

//! marshalling class for AppInterfaceUnregisteredReason

  class AppInterfaceUnregisteredReasonMarshaller
  {
  public:
  
    static std::string toName(const AppInterfaceUnregisteredReason& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(AppInterfaceUnregisteredReason& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=AppInterfaceUnregisteredReason::INVALID_ENUM;
    }
  
    static bool checkIntegrity(AppInterfaceUnregisteredReason& e)		{ return e.mInternal!=AppInterfaceUnregisteredReason::INVALID_ENUM; } 
    static bool checkIntegrityConst(const AppInterfaceUnregisteredReason& e)	{ return e.mInternal!=AppInterfaceUnregisteredReason::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,AppInterfaceUnregisteredReason& e);
    static const std::string toString(const AppInterfaceUnregisteredReason& e);
  
    static bool fromJSON(const Json::Value& s,AppInterfaceUnregisteredReason& e);
    static Json::Value toJSON(const AppInterfaceUnregisteredReason& e);
  
    static const char* getName(AppInterfaceUnregisteredReason::AppInterfaceUnregisteredReasonInternal e)
    {
       return (e>=0 && e<11) ? mHashTable[e].name : NULL;
    }
  
    static const AppInterfaceUnregisteredReason::AppInterfaceUnregisteredReasonInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[11];
  };
  
}

#endif
