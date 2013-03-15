//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPC_TEXTALIGNMENTMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPC_TEXTALIGNMENTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/SDLRPCObjects/V1/TextAlignment.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPC
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
