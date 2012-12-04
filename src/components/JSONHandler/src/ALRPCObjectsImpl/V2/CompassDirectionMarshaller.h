#ifndef NSAPPLINKRPCV2_COMPASSDIRECTIONMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_COMPASSDIRECTIONMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V2/CompassDirection.h"


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

//! marshalling class for CompassDirection

  class CompassDirectionMarshaller
  {
  public:
  
    static std::string toName(const CompassDirection& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(CompassDirection& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=CompassDirection::INVALID_ENUM;
    }
  
    static bool checkIntegrity(CompassDirection& e)		{ return e.mInternal!=CompassDirection::INVALID_ENUM; } 
    static bool checkIntegrityConst(const CompassDirection& e)	{ return e.mInternal!=CompassDirection::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,CompassDirection& e);
    static const std::string toString(const CompassDirection& e);
  
    static bool fromJSON(const Json::Value& s,CompassDirection& e);
    static Json::Value toJSON(const CompassDirection& e);
  
    static const char* getName(CompassDirection::CompassDirectionInternal e)
    {
       return (e>=0 && e<8) ? mHashTable[e].name : NULL;
    }
  
    static const CompassDirection::CompassDirectionInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[8];
  };
  
}

#endif
