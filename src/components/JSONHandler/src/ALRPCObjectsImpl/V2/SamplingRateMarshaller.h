#ifndef NSAPPLINKRPCV2_SAMPLINGRATEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_SAMPLINGRATEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V2/SamplingRate.h"


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

//! marshalling class for SamplingRate

  class SamplingRateMarshaller
  {
  public:
  
    static std::string toName(const SamplingRate& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(SamplingRate& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=SamplingRate::INVALID_ENUM;
    }
  
    static bool checkIntegrity(SamplingRate& e)		{ return e.mInternal!=SamplingRate::INVALID_ENUM; } 
    static bool checkIntegrityConst(const SamplingRate& e)	{ return e.mInternal!=SamplingRate::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,SamplingRate& e);
    static const std::string toString(const SamplingRate& e);
  
    static bool fromJSON(const Json::Value& s,SamplingRate& e);
    static Json::Value toJSON(const SamplingRate& e);
  
    static const char* getName(SamplingRate::SamplingRateInternal e)
    {
       return (e>=0 && e<4) ? mHashTable[e].name : NULL;
    }
  
    static const SamplingRate::SamplingRateInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[4];
  };
  
}

#endif
