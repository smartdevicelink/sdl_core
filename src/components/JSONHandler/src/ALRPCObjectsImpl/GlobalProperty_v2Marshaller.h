#ifndef NSAPPLINKRPC_GLOBALPROPERTY_V2MARSHALLER_INCLUDE
#define NSAPPLINKRPC_GLOBALPROPERTY_V2MARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/GlobalProperty_v2.h"


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

//! marshalling class for GlobalProperty_v2

  class GlobalProperty_v2Marshaller
  {
  public:
  
    static std::string toName(const GlobalProperty_v2& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(GlobalProperty_v2& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=GlobalProperty_v2::INVALID_ENUM;
    }
  
    static bool checkIntegrity(GlobalProperty_v2& e)		{ return e.mInternal!=GlobalProperty_v2::INVALID_ENUM; } 
    static bool checkIntegrityConst(const GlobalProperty_v2& e)	{ return e.mInternal!=GlobalProperty_v2::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,GlobalProperty_v2& e);
    static const std::string toString(const GlobalProperty_v2& e);
  
    static bool fromJSON(const Json::Value& s,GlobalProperty_v2& e);
    static Json::Value toJSON(const GlobalProperty_v2& e);
  
    static const char* getName(GlobalProperty_v2::GlobalProperty_v2Internal e)
    {
       return (e>=0 && e<4) ? mHashTable[e].name : NULL;
    }
  
    static const GlobalProperty_v2::GlobalProperty_v2Internal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[4];
  };
  
}

#endif
