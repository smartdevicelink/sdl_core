//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_TIMERMODEMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_TIMERMODEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/TimerMode.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPCV2
{

//! marshalling class for TimerMode

  class TimerModeMarshaller
  {
  public:
  
    static std::string toName(const TimerMode& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(TimerMode& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=TimerMode::INVALID_ENUM;
    }
  
    static bool checkIntegrity(TimerMode& e)		{ return e.mInternal!=TimerMode::INVALID_ENUM; } 
    static bool checkIntegrityConst(const TimerMode& e)	{ return e.mInternal!=TimerMode::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,TimerMode& e);
    static const std::string toString(const TimerMode& e);
  
    static bool fromJSON(const Json::Value& s,TimerMode& e);
    static Json::Value toJSON(const TimerMode& e);
  
    static const char* getName(TimerMode::TimerModeInternal e)
    {
       return (e>=0 && e<3) ? mHashTable[e].name : NULL;
    }
  
    static const TimerMode::TimerModeInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[3];
  };
  
}

#endif
