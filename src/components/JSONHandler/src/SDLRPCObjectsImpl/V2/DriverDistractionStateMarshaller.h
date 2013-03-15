//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_DRIVERDISTRACTIONSTATEMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_DRIVERDISTRACTIONSTATEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/DriverDistractionState.h"


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
