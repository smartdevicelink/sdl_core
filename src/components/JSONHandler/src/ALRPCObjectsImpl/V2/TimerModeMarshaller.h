#ifndef NSAPPLINKRPCV2_TIMERMODEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_TIMERMODEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V2/TimerMode.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
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
