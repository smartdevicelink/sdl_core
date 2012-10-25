#ifndef TIMERMODEMARSHALLER_INCLUDE
#define TIMERMODEMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/TimerMode.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


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

#endif
