//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_SAMPLINGRATEMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_SAMPLINGRATEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/SamplingRate.h"


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
