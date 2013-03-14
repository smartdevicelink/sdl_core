#ifndef NSAPPLINKRPCV2_CHARACTERSETMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_CHARACTERSETMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/CharacterSet.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsSmartDeviceLinkRPCV2
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
