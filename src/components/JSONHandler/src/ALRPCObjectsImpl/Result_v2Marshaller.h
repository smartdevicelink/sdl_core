#ifndef NSAPPLINKRPC_RESULT_V2MARSHALLER_INCLUDE
#define NSAPPLINKRPC_RESULT_V2MARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/Result_v2.h"


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

//! marshalling class for Result_v2

  class Result_v2Marshaller
  {
  public:
  
    static std::string toName(const Result_v2& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(Result_v2& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=Result_v2::INVALID_ENUM;
    }
  
    static bool checkIntegrity(Result_v2& e)		{ return e.mInternal!=Result_v2::INVALID_ENUM; } 
    static bool checkIntegrityConst(const Result_v2& e)	{ return e.mInternal!=Result_v2::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,Result_v2& e);
    static const std::string toString(const Result_v2& e);
  
    static bool fromJSON(const Json::Value& s,Result_v2& e);
    static Json::Value toJSON(const Result_v2& e);
  
    static const char* getName(Result_v2::Result_v2Internal e)
    {
       return (e>=0 && e<29) ? mHashTable[e].name : NULL;
    }
  
    static const Result_v2::Result_v2Internal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[29];
  };
  
}

#endif
