#ifndef NSAPPLINKRPC_TEXTALIGNMENTMARSHALLER_INCLUDE
#define NSAPPLINKRPC_TEXTALIGNMENTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/TextAlignment.h"


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

//! marshalling class for TextAlignment

  class TextAlignmentMarshaller
  {
  public:
  
    static std::string toName(const TextAlignment& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(TextAlignment& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=TextAlignment::INVALID_ENUM;
    }
  
    static bool checkIntegrity(TextAlignment& e)		{ return e.mInternal!=TextAlignment::INVALID_ENUM; } 
    static bool checkIntegrityConst(const TextAlignment& e)	{ return e.mInternal!=TextAlignment::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,TextAlignment& e);
    static const std::string toString(const TextAlignment& e);
  
    static bool fromJSON(const Json::Value& s,TextAlignment& e);
    static Json::Value toJSON(const TextAlignment& e);
  
    static const char* getName(TextAlignment::TextAlignmentInternal e)
    {
       return (e>=0 && e<3) ? mHashTable[e].name : NULL;
    }
  
    static const TextAlignment::TextAlignmentInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[3];
  };
  
}

#endif
