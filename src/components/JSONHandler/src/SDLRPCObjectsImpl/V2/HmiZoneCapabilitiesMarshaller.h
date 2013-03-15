//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_HMIZONECAPABILITIESMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_HMIZONECAPABILITIESMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/HmiZoneCapabilities.h"


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

//! marshalling class for HmiZoneCapabilities

  class HmiZoneCapabilitiesMarshaller
  {
  public:
  
    static std::string toName(const HmiZoneCapabilities& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(HmiZoneCapabilities& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=HmiZoneCapabilities::INVALID_ENUM;
    }
  
    static bool checkIntegrity(HmiZoneCapabilities& e)		{ return e.mInternal!=HmiZoneCapabilities::INVALID_ENUM; } 
    static bool checkIntegrityConst(const HmiZoneCapabilities& e)	{ return e.mInternal!=HmiZoneCapabilities::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,HmiZoneCapabilities& e);
    static const std::string toString(const HmiZoneCapabilities& e);
  
    static bool fromJSON(const Json::Value& s,HmiZoneCapabilities& e);
    static Json::Value toJSON(const HmiZoneCapabilities& e);
  
    static const char* getName(HmiZoneCapabilities::HmiZoneCapabilitiesInternal e)
    {
       return (e>=0 && e<2) ? mHashTable[e].name : NULL;
    }
  
    static const HmiZoneCapabilities::HmiZoneCapabilitiesInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[2];
  };
  
}

#endif
