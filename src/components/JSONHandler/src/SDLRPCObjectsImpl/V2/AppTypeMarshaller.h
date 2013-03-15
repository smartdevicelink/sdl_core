//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_APPTYPEMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_APPTYPEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/AppType.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPCV2
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
