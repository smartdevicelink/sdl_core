#ifndef NSAPPLINKRPCV2_SINGLETIREPRESSURESTATUSMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_SINGLETIREPRESSURESTATUSMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V2/SingleTirePressureStatus.h"


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

//! marshalling class for SingleTirePressureStatus

  class SingleTirePressureStatusMarshaller
  {
  public:
  
    static std::string toName(const SingleTirePressureStatus& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(SingleTirePressureStatus& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=SingleTirePressureStatus::INVALID_ENUM;
    }
  
    static bool checkIntegrity(SingleTirePressureStatus& e)		{ return e.mInternal!=SingleTirePressureStatus::INVALID_ENUM; } 
    static bool checkIntegrityConst(const SingleTirePressureStatus& e)	{ return e.mInternal!=SingleTirePressureStatus::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,SingleTirePressureStatus& e);
    static const std::string toString(const SingleTirePressureStatus& e);
  
    static bool fromJSON(const Json::Value& s,SingleTirePressureStatus& e);
    static Json::Value toJSON(const SingleTirePressureStatus& e);
  
    static const char* getName(SingleTirePressureStatus::SingleTirePressureStatusInternal e)
    {
       return (e>=0 && e<5) ? mHashTable[e].name : NULL;
    }
  
    static const SingleTirePressureStatus::SingleTirePressureStatusInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[5];
  };
  
}

#endif
