#ifndef NSAPPLINKRPC_LANGUAGE_V2MARSHALLER_INCLUDE
#define NSAPPLINKRPC_LANGUAGE_V2MARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/Language_v2.h"


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

//! marshalling class for Language_v2

  class Language_v2Marshaller
  {
  public:
  
    static std::string toName(const Language_v2& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(Language_v2& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=Language_v2::INVALID_ENUM;
    }
  
    static bool checkIntegrity(Language_v2& e)		{ return e.mInternal!=Language_v2::INVALID_ENUM; } 
    static bool checkIntegrityConst(const Language_v2& e)	{ return e.mInternal!=Language_v2::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,Language_v2& e);
    static const std::string toString(const Language_v2& e);
  
    static bool fromJSON(const Json::Value& s,Language_v2& e);
    static Json::Value toJSON(const Language_v2& e);
  
    static const char* getName(Language_v2::Language_v2Internal e)
    {
       return (e>=0 && e<20) ? mHashTable[e].name : NULL;
    }
  
    static const Language_v2::Language_v2Internal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[20];
  };
  
}

#endif
