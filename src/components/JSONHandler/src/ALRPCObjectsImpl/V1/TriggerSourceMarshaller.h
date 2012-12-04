#ifndef NSAPPLINKRPCV1_TRIGGERSOURCEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV1_TRIGGERSOURCEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V1/TriggerSource.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 14:55:10 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV1
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
