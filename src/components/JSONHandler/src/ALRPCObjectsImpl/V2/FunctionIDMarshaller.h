#ifndef NSAPPLINKRPCV2_FUNCTIONIDMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_FUNCTIONIDMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V2/FunctionID.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

//! marshalling class for FunctionID

  class FunctionIDMarshaller
  {
  public:
  
    static std::string toName(const FunctionID& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(FunctionID& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=FunctionID::INVALID_ENUM;
    }
  
    static bool checkIntegrity(FunctionID& e)		{ return e.mInternal!=FunctionID::INVALID_ENUM; } 
    static bool checkIntegrityConst(const FunctionID& e)	{ return e.mInternal!=FunctionID::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,FunctionID& e);
    static const std::string toString(const FunctionID& e);
  
    static bool fromJSON(const Json::Value& s,FunctionID& e);
    static Json::Value toJSON(const FunctionID& e);
  
    static const char* getName(FunctionID::FunctionIDInternal e)
    {
       return (e>=0 && e<51) ? mHashTable[e].name : NULL;
    }
  
    static const FunctionID::FunctionIDInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[51];
  };
  
}

#endif
