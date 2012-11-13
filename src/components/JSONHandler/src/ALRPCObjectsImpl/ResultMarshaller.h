#ifndef APPLINKRPC_RESULTMARSHALLER_INCLUDE
#define APPLINKRPC_RESULTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/Result.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Nov  7 13:10:41 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/

namespace AppLinkRPC
{

//! marshalling class for Result

  class ResultMarshaller
  {
  public:
  
    static std::string toName(const Result& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(Result& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=Result::INVALID_ENUM;
    }
  
    static bool checkIntegrity(Result& e)		{ return e.mInternal!=Result::INVALID_ENUM; } 
    static bool checkIntegrityConst(const Result& e)	{ return e.mInternal!=Result::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,Result& e);
    static const std::string toString(const Result& e);
  
    static bool fromJSON(const Json::Value& s,Result& e);
    static Json::Value toJSON(const Result& e);
  
    static const char* getName(Result::ResultInternal e)
    {
       return (e>=0 && e<20) ? mHashTable[e].name : NULL;
    }
  
    static const Result::ResultInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[20];
  };
  
}

#endif
