#ifndef NSAPPLINKRPC_SOFTBUTTONTYPEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_SOFTBUTTONTYPEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/SoftButtonType.h"


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
