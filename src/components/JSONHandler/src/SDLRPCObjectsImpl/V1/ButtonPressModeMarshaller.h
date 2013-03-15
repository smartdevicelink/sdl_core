//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPC_BUTTONPRESSMODEMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPC_BUTTONPRESSMODEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/SDLRPCObjects/V1/ButtonPressMode.h"


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

//! marshalling class for ButtonPressMode

  class ButtonPressModeMarshaller
  {
  public:
  
    static std::string toName(const ButtonPressMode& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(ButtonPressMode& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=ButtonPressMode::INVALID_ENUM;
    }
  
    static bool checkIntegrity(ButtonPressMode& e)		{ return e.mInternal!=ButtonPressMode::INVALID_ENUM; } 
    static bool checkIntegrityConst(const ButtonPressMode& e)	{ return e.mInternal!=ButtonPressMode::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,ButtonPressMode& e);
    static const std::string toString(const ButtonPressMode& e);
  
    static bool fromJSON(const Json::Value& s,ButtonPressMode& e);
    static Json::Value toJSON(const ButtonPressMode& e);
  
    static const char* getName(ButtonPressMode::ButtonPressModeInternal e)
    {
       return (e>=0 && e<2) ? mHashTable[e].name : NULL;
    }
  
    static const ButtonPressMode::ButtonPressModeInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[2];
  };
  
}

#endif
