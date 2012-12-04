#ifndef NSAPPLINKRPCV2_APPTYPEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_APPTYPEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V2/AppType.h"


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

//! marshalling class for AppType

  class AppTypeMarshaller
  {
  public:
  
    static std::string toName(const AppType& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(AppType& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=AppType::INVALID_ENUM;
    }
  
    static bool checkIntegrity(AppType& e)		{ return e.mInternal!=AppType::INVALID_ENUM; } 
    static bool checkIntegrityConst(const AppType& e)	{ return e.mInternal!=AppType::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,AppType& e);
    static const std::string toString(const AppType& e);
  
    static bool fromJSON(const Json::Value& s,AppType& e);
    static Json::Value toJSON(const AppType& e);
  
    static const char* getName(AppType::AppTypeInternal e)
    {
       return (e>=0 && e<10) ? mHashTable[e].name : NULL;
    }
  
    static const AppType::AppTypeInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[10];
  };
  
}

#endif
