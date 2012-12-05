#ifndef NSAPPLINKRPC_TBTSTATEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_TBTSTATEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V1/TBTState.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 16:02:39 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

//! marshalling class for TBTState

  class TBTStateMarshaller
  {
  public:
  
    static std::string toName(const TBTState& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(TBTState& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=TBTState::INVALID_ENUM;
    }
  
    static bool checkIntegrity(TBTState& e)		{ return e.mInternal!=TBTState::INVALID_ENUM; } 
    static bool checkIntegrityConst(const TBTState& e)	{ return e.mInternal!=TBTState::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,TBTState& e);
    static const std::string toString(const TBTState& e);
  
    static bool fromJSON(const Json::Value& s,TBTState& e);
    static Json::Value toJSON(const TBTState& e);
  
    static const char* getName(TBTState::TBTStateInternal e)
    {
       return (e>=0 && e<4) ? mHashTable[e].name : NULL;
    }
  
    static const TBTState::TBTStateInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[4];
  };
  
}

#endif
