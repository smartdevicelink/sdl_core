//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPC_VRCAPABILITIESMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPC_VRCAPABILITIESMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/SDLRPCObjects/V1/VrCapabilities.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPC
{

//! marshalling class for VrCapabilities

  class VrCapabilitiesMarshaller
  {
  public:
  
    static std::string toName(const VrCapabilities& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(VrCapabilities& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=VrCapabilities::INVALID_ENUM;
    }
  
    static bool checkIntegrity(VrCapabilities& e)		{ return e.mInternal!=VrCapabilities::INVALID_ENUM; } 
    static bool checkIntegrityConst(const VrCapabilities& e)	{ return e.mInternal!=VrCapabilities::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,VrCapabilities& e);
    static const std::string toString(const VrCapabilities& e);
  
    static bool fromJSON(const Json::Value& s,VrCapabilities& e);
    static Json::Value toJSON(const VrCapabilities& e);
  
    static const char* getName(VrCapabilities::VrCapabilitiesInternal e)
    {
       return (e>=0 && e<1) ? mHashTable[e].name : NULL;
    }
  
    static const VrCapabilities::VrCapabilitiesInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[1];
  };
  
}

#endif
