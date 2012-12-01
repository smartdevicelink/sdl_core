#ifndef NSAPPLINKRPC_PRNDLMARSHALLER_INCLUDE
#define NSAPPLINKRPC_PRNDLMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/PRNDL.h"


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

//! marshalling class for PRNDL

  class PRNDLMarshaller
  {
  public:
  
    static std::string toName(const PRNDL& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(PRNDL& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=PRNDL::INVALID_ENUM;
    }
  
    static bool checkIntegrity(PRNDL& e)		{ return e.mInternal!=PRNDL::INVALID_ENUM; } 
    static bool checkIntegrityConst(const PRNDL& e)	{ return e.mInternal!=PRNDL::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,PRNDL& e);
    static const std::string toString(const PRNDL& e);
  
    static bool fromJSON(const Json::Value& s,PRNDL& e);
    static Json::Value toJSON(const PRNDL& e);
  
    static const char* getName(PRNDL::PRNDLInternal e)
    {
       return (e>=0 && e<14) ? mHashTable[e].name : NULL;
    }
  
    static const PRNDL::PRNDLInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[14];
  };
  
}

#endif
