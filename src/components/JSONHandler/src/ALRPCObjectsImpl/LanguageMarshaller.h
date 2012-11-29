#ifndef NSAPPLINKRPC_LANGUAGEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_LANGUAGEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/Language.h"


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

//! marshalling class for Language

  class LanguageMarshaller
  {
  public:
  
    static std::string toName(const Language& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(Language& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=Language::INVALID_ENUM;
    }
  
    static bool checkIntegrity(Language& e)		{ return e.mInternal!=Language::INVALID_ENUM; } 
    static bool checkIntegrityConst(const Language& e)	{ return e.mInternal!=Language::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,Language& e);
    static const std::string toString(const Language& e);
  
    static bool fromJSON(const Json::Value& s,Language& e);
    static Json::Value toJSON(const Language& e);
  
    static const char* getName(Language::LanguageInternal e)
    {
       return (e>=0 && e<3) ? mHashTable[e].name : NULL;
    }
  
    static const Language::LanguageInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[3];
  };
  
}

#endif
