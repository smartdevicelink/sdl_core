#ifndef NSAPPLINKRPC_SAMPLINGRATEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_SAMPLINGRATEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/SamplingRate.h"


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
