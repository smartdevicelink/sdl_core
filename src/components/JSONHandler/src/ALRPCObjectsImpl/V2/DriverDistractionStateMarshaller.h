#ifndef NSAPPLINKRPCV2_DRIVERDISTRACTIONSTATEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_DRIVERDISTRACTIONSTATEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V2/DriverDistractionState.h"


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

//! marshalling class for DriverDistractionState

  class DriverDistractionStateMarshaller
  {
  public:
  
    static std::string toName(const DriverDistractionState& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(DriverDistractionState& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=DriverDistractionState::INVALID_ENUM;
    }
  
    static bool checkIntegrity(DriverDistractionState& e)		{ return e.mInternal!=DriverDistractionState::INVALID_ENUM; } 
    static bool checkIntegrityConst(const DriverDistractionState& e)	{ return e.mInternal!=DriverDistractionState::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,DriverDistractionState& e);
    static const std::string toString(const DriverDistractionState& e);
  
    static bool fromJSON(const Json::Value& s,DriverDistractionState& e);
    static Json::Value toJSON(const DriverDistractionState& e);
  
    static const char* getName(DriverDistractionState::DriverDistractionStateInternal e)
    {
       return (e>=0 && e<2) ? mHashTable[e].name : NULL;
    }
  
    static const DriverDistractionState::DriverDistractionStateInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[2];
  };
  
}

#endif
