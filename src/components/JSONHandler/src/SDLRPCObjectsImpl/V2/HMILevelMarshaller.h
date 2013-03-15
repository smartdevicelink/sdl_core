//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_HMILEVELMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_HMILEVELMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/HMILevel.h"


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

//! marshalling class for HMILevel

  class HMILevelMarshaller
  {
  public:
  
    static std::string toName(const HMILevel& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(HMILevel& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=HMILevel::INVALID_ENUM;
    }
  
    static bool checkIntegrity(HMILevel& e)		{ return e.mInternal!=HMILevel::INVALID_ENUM; } 
    static bool checkIntegrityConst(const HMILevel& e)	{ return e.mInternal!=HMILevel::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,HMILevel& e);
    static const std::string toString(const HMILevel& e);
  
    static bool fromJSON(const Json::Value& s,HMILevel& e);
    static Json::Value toJSON(const HMILevel& e);
  
    static const char* getName(HMILevel::HMILevelInternal e)
    {
       return (e>=0 && e<4) ? mHashTable[e].name : NULL;
    }
  
    static const HMILevel::HMILevelInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[4];
  };
  
}

#endif
