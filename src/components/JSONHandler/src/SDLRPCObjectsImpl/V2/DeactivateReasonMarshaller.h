#ifndef NSAPPLINKRPCV2_DEACTIVATEREASONMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_DEACTIVATEREASONMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/DeactivateReason.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsSmartDeviceLinkRPCV2
{

//! marshalling class for DeactivateReason

  class DeactivateReasonMarshaller
  {
  public:
  
    static std::string toName(const DeactivateReason& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(DeactivateReason& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=DeactivateReason::INVALID_ENUM;
    }
  
    static bool checkIntegrity(DeactivateReason& e)		{ return e.mInternal!=DeactivateReason::INVALID_ENUM; } 
    static bool checkIntegrityConst(const DeactivateReason& e)	{ return e.mInternal!=DeactivateReason::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,DeactivateReason& e);
    static const std::string toString(const DeactivateReason& e);
  
    static bool fromJSON(const Json::Value& s,DeactivateReason& e);
    static Json::Value toJSON(const DeactivateReason& e);
  
    static const char* getName(DeactivateReason::DeactivateReasonInternal e)
    {
       return (e>=0 && e<6) ? mHashTable[e].name : NULL;
    }
  
    static const DeactivateReason::DeactivateReasonInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[6];
  };
  
}

#endif
