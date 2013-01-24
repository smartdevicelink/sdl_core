#ifndef NSAPPLINKRPCV2_SOFTBUTTONTYPEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_SOFTBUTTONTYPEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V2/SoftButtonType.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

//! marshalling class for SoftButtonType

  class SoftButtonTypeMarshaller
  {
  public:
  
    static std::string toName(const SoftButtonType& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(SoftButtonType& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=SoftButtonType::INVALID_ENUM;
    }
  
    static bool checkIntegrity(SoftButtonType& e)		{ return e.mInternal!=SoftButtonType::INVALID_ENUM; } 
    static bool checkIntegrityConst(const SoftButtonType& e)	{ return e.mInternal!=SoftButtonType::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,SoftButtonType& e);
    static const std::string toString(const SoftButtonType& e);
  
    static bool fromJSON(const Json::Value& s,SoftButtonType& e);
    static Json::Value toJSON(const SoftButtonType& e);
  
    static const char* getName(SoftButtonType::SoftButtonTypeInternal e)
    {
       return (e>=0 && e<3) ? mHashTable[e].name : NULL;
    }
  
    static const SoftButtonType::SoftButtonTypeInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[3];
  };
  
}

#endif
