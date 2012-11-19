#ifndef NSAPPLINKRPC_CHARACTERSETMARSHALLER_INCLUDE
#define NSAPPLINKRPC_CHARACTERSETMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/CharacterSet.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Mon Nov 19 10:37:06 2012
  source stamp	Mon Nov 19 10:35:56 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

//! marshalling class for CharacterSet

  class CharacterSetMarshaller
  {
  public:
  
    static std::string toName(const CharacterSet& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(CharacterSet& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=CharacterSet::INVALID_ENUM;
    }
  
    static bool checkIntegrity(CharacterSet& e)		{ return e.mInternal!=CharacterSet::INVALID_ENUM; } 
    static bool checkIntegrityConst(const CharacterSet& e)	{ return e.mInternal!=CharacterSet::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,CharacterSet& e);
    static const std::string toString(const CharacterSet& e);
  
    static bool fromJSON(const Json::Value& s,CharacterSet& e);
    static Json::Value toJSON(const CharacterSet& e);
  
    static const char* getName(CharacterSet::CharacterSetInternal e)
    {
       return (e>=0 && e<4) ? mHashTable[e].name : NULL;
    }
  
    static const CharacterSet::CharacterSetInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[4];
  };
  
}

#endif
