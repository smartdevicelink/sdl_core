#ifndef DIMENSIONMARSHALLER_INCLUDE
#define DIMENSIONMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/Dimension.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
  author	robok0der
*/


//! marshalling class for Dimension

class DimensionMarshaller
{
public:

  static std::string toName(const Dimension& e) 	{ return getName(e.mInternal) ?: ""; }

  static bool fromName(Dimension& e,const std::string& s)
  { 
    return (e.mInternal=getIndex(s.c_str()))!=Dimension::INVALID_ENUM;
  }

  static bool checkIntegrity(Dimension& e)		{ return e.mInternal!=Dimension::INVALID_ENUM; } 
  static bool checkIntegrityConst(const Dimension& e)	{ return e.mInternal!=Dimension::INVALID_ENUM; } 

  static bool fromString(const std::string& s,Dimension& e);
  static const std::string toString(const Dimension& e);

  static bool fromJSON(const Json::Value& s,Dimension& e);
  static Json::Value toJSON(const Dimension& e);

  static const char* getName(Dimension::DimensionInternal e)
  {
     return (e>=0 && e<3) ? mHashTable[e].name : NULL;
  }

  static const Dimension::DimensionInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[3];
};

#endif
