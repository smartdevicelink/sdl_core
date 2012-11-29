#ifndef NSAPPLINKRPC_HMILEVELMARSHALLER_INCLUDE
#define NSAPPLINKRPC_HMILEVELMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/HMILevel.h"


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
