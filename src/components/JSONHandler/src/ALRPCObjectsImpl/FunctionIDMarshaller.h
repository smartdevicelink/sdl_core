#ifndef FUNCTIONIDMARSHALLER_INCLUDE
#define FUNCTIONIDMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/FunctionID.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


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

#endif
