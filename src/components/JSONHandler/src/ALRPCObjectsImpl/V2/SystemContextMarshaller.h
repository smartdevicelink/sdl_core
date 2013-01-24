#ifndef NSAPPLINKRPCV2_SYSTEMCONTEXTMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_SYSTEMCONTEXTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V2/SystemContext.h"


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

//! marshalling class for SystemContext

  class SystemContextMarshaller
  {
  public:
  
    static std::string toName(const SystemContext& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(SystemContext& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=SystemContext::INVALID_ENUM;
    }
  
    static bool checkIntegrity(SystemContext& e)		{ return e.mInternal!=SystemContext::INVALID_ENUM; } 
    static bool checkIntegrityConst(const SystemContext& e)	{ return e.mInternal!=SystemContext::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,SystemContext& e);
    static const std::string toString(const SystemContext& e);
  
    static bool fromJSON(const Json::Value& s,SystemContext& e);
    static Json::Value toJSON(const SystemContext& e);
  
    static const char* getName(SystemContext::SystemContextInternal e)
    {
       return (e>=0 && e<5) ? mHashTable[e].name : NULL;
    }
  
    static const SystemContext::SystemContextInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[5];
  };
  
}

#endif
