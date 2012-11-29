#ifndef NSAPPLINKRPC_SYSTEMCONTEXT_V2MARSHALLER_INCLUDE
#define NSAPPLINKRPC_SYSTEMCONTEXT_V2MARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/SystemContext_v2.h"


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

//! marshalling class for SystemContext_v2

  class SystemContext_v2Marshaller
  {
  public:
  
    static std::string toName(const SystemContext_v2& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(SystemContext_v2& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=SystemContext_v2::INVALID_ENUM;
    }
  
    static bool checkIntegrity(SystemContext_v2& e)		{ return e.mInternal!=SystemContext_v2::INVALID_ENUM; } 
    static bool checkIntegrityConst(const SystemContext_v2& e)	{ return e.mInternal!=SystemContext_v2::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,SystemContext_v2& e);
    static const std::string toString(const SystemContext_v2& e);
  
    static bool fromJSON(const Json::Value& s,SystemContext_v2& e);
    static Json::Value toJSON(const SystemContext_v2& e);
  
    static const char* getName(SystemContext_v2::SystemContext_v2Internal e)
    {
       return (e>=0 && e<5) ? mHashTable[e].name : NULL;
    }
  
    static const SystemContext_v2::SystemContext_v2Internal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[5];
  };
  
}

#endif
