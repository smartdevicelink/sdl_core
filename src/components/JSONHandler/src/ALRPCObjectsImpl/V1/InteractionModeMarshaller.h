#ifndef NSAPPLINKRPC_INTERACTIONMODEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_INTERACTIONMODEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V1/InteractionMode.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	robok0der
*/

namespace NsAppLinkRPC
{

//! marshalling class for InteractionMode

  class InteractionModeMarshaller
  {
  public:
  
    static std::string toName(const InteractionMode& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(InteractionMode& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=InteractionMode::INVALID_ENUM;
    }
  
    static bool checkIntegrity(InteractionMode& e)		{ return e.mInternal!=InteractionMode::INVALID_ENUM; } 
    static bool checkIntegrityConst(const InteractionMode& e)	{ return e.mInternal!=InteractionMode::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,InteractionMode& e);
    static const std::string toString(const InteractionMode& e);
  
    static bool fromJSON(const Json::Value& s,InteractionMode& e);
    static Json::Value toJSON(const InteractionMode& e);
  
    static const char* getName(InteractionMode::InteractionModeInternal e)
    {
       return (e>=0 && e<3) ? mHashTable[e].name : NULL;
    }
  
    static const InteractionMode::InteractionModeInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[3];
  };
  
}

#endif
