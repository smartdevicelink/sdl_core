#ifndef DRIVERDISTRACTIONSTATEMARSHALLER_INCLUDE
#define DRIVERDISTRACTIONSTATEMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/DriverDistractionState.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Oct 30 08:29:32 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/


//! marshalling class for DriverDistractionState

class DriverDistractionStateMarshaller
{
public:

  static std::string toName(const DriverDistractionState& e) 	{ return getName(e.mInternal) ?: ""; }

  static bool fromName(DriverDistractionState& e,const std::string& s)
  { 
    return (e.mInternal=getIndex(s.c_str()))!=DriverDistractionState::INVALID_ENUM;
  }

  static bool checkIntegrity(DriverDistractionState& e)		{ return e.mInternal!=DriverDistractionState::INVALID_ENUM; } 
  static bool checkIntegrityConst(const DriverDistractionState& e)	{ return e.mInternal!=DriverDistractionState::INVALID_ENUM; } 

  static bool fromString(const std::string& s,DriverDistractionState& e);
  static const std::string toString(const DriverDistractionState& e);

  static bool fromJSON(const Json::Value& s,DriverDistractionState& e);
  static Json::Value toJSON(const DriverDistractionState& e);

  static const char* getName(DriverDistractionState::DriverDistractionStateInternal e)
  {
     return (e>=0 && e<2) ? mHashTable[e].name : NULL;
  }

  static const DriverDistractionState::DriverDistractionStateInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[2];
};

#endif
