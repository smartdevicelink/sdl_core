#ifndef SINGLETIREPRESSURESTATUSMARSHALLER_INCLUDE
#define SINGLETIREPRESSURESTATUSMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/SingleTirePressureStatus.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


//! marshalling class for SingleTirePressureStatus

class SingleTirePressureStatusMarshaller
{
public:

  static std::string toName(const SingleTirePressureStatus& e) 	{ return getName(e.mInternal) ?: ""; }

  static bool fromName(SingleTirePressureStatus& e,const std::string& s)
  { 
    return (e.mInternal=getIndex(s.c_str()))!=SingleTirePressureStatus::INVALID_ENUM;
  }

  static bool checkIntegrity(SingleTirePressureStatus& e)		{ return e.mInternal!=SingleTirePressureStatus::INVALID_ENUM; } 
  static bool checkIntegrityConst(const SingleTirePressureStatus& e)	{ return e.mInternal!=SingleTirePressureStatus::INVALID_ENUM; } 

  static bool fromString(const std::string& s,SingleTirePressureStatus& e);
  static const std::string toString(const SingleTirePressureStatus& e);

  static bool fromJSON(const Json::Value& s,SingleTirePressureStatus& e);
  static Json::Value toJSON(const SingleTirePressureStatus& e);

  static const char* getName(SingleTirePressureStatus::SingleTirePressureStatusInternal e)
  {
     return (e>=0 && e<5) ? mHashTable[e].name : NULL;
  }

  static const SingleTirePressureStatus::SingleTirePressureStatusInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[5];
};

#endif
