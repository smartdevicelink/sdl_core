#ifndef NSAPPLINKRPC_BUTTONEVENTMODEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_BUTTONEVENTMODEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/ButtonEventMode.h"


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

//! marshalling class for ButtonEventMode

  class ButtonEventModeMarshaller
  {
  public:
  
    static std::string toName(const ButtonEventMode& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(ButtonEventMode& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=ButtonEventMode::INVALID_ENUM;
    }
  
    static bool checkIntegrity(ButtonEventMode& e)		{ return e.mInternal!=ButtonEventMode::INVALID_ENUM; } 
    static bool checkIntegrityConst(const ButtonEventMode& e)	{ return e.mInternal!=ButtonEventMode::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,ButtonEventMode& e);
    static const std::string toString(const ButtonEventMode& e);
  
    static bool fromJSON(const Json::Value& s,ButtonEventMode& e);
    static Json::Value toJSON(const ButtonEventMode& e);
  
    static const char* getName(ButtonEventMode::ButtonEventModeInternal e)
    {
       return (e>=0 && e<2) ? mHashTable[e].name : NULL;
    }
  
    static const ButtonEventMode::ButtonEventModeInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[2];
  };
  
}

#endif
