#ifndef NSAPPLINKRPCV2_INTERACTIONMODEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_INTERACTIONMODEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V2/InteractionMode.h"


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
