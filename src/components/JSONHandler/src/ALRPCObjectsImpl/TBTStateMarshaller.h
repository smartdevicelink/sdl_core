#ifndef TBTSTATEMARSHALLER_INCLUDE
#define TBTSTATEMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/TBTState.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


//! marshalling class for TBTState

class TBTStateMarshaller
{
public:

  static std::string toName(const TBTState& e) 	{ return getName(e.mInternal) ?: ""; }

  static bool fromName(TBTState& e,const std::string& s)
  { 
    return (e.mInternal=getIndex(s.c_str()))!=TBTState::INVALID_ENUM;
  }

  static bool checkIntegrity(TBTState& e)		{ return e.mInternal!=TBTState::INVALID_ENUM; } 
  static bool checkIntegrityConst(const TBTState& e)	{ return e.mInternal!=TBTState::INVALID_ENUM; } 

  static bool fromString(const std::string& s,TBTState& e);
  static const std::string toString(const TBTState& e);

  static bool fromJSON(const Json::Value& s,TBTState& e);
  static Json::Value toJSON(const TBTState& e);

  static const char* getName(TBTState::TBTStateInternal e)
  {
     return (e>=0 && e<4) ? mHashTable[e].name : NULL;
  }

  static const TBTState::TBTStateInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[4];
};

#endif
