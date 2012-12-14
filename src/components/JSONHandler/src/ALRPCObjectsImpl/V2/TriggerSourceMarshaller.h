#ifndef NSAPPLINKRPCV2_TRIGGERSOURCEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_TRIGGERSOURCEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V2/TriggerSource.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

//! marshalling class for TriggerSource

  class TriggerSourceMarshaller
  {
  public:
  
    static std::string toName(const TriggerSource& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(TriggerSource& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=TriggerSource::INVALID_ENUM;
    }
  
    static bool checkIntegrity(TriggerSource& e)		{ return e.mInternal!=TriggerSource::INVALID_ENUM; } 
    static bool checkIntegrityConst(const TriggerSource& e)	{ return e.mInternal!=TriggerSource::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,TriggerSource& e);
    static const std::string toString(const TriggerSource& e);
  
    static bool fromJSON(const Json::Value& s,TriggerSource& e);
    static Json::Value toJSON(const TriggerSource& e);
  
    static const char* getName(TriggerSource::TriggerSourceInternal e)
    {
       return (e>=0 && e<2) ? mHashTable[e].name : NULL;
    }
  
    static const TriggerSource::TriggerSourceInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[2];
  };
  
}

#endif
