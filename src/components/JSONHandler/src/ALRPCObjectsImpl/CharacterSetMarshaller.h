#ifndef CHARACTERSETMARSHALLER_INCLUDE
#define CHARACTERSETMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/CharacterSet.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
  author	robok0der
*/


//! marshalling class for CharacterSet

class CharacterSetMarshaller
{
public:

  static std::string toName(const CharacterSet& e) 	{ return getName(e.mInternal) ?: ""; }

  static bool fromName(CharacterSet& e,const std::string& s)
  { 
    return (e.mInternal=getIndex(s.c_str()))!=CharacterSet::INVALID_ENUM;
  }

  static bool checkIntegrity(CharacterSet& e)		{ return e.mInternal!=CharacterSet::INVALID_ENUM; } 
  static bool checkIntegrityConst(const CharacterSet& e)	{ return e.mInternal!=CharacterSet::INVALID_ENUM; } 

  static bool fromString(const std::string& s,CharacterSet& e);
  static const std::string toString(const CharacterSet& e);

  static bool fromJSON(const Json::Value& s,CharacterSet& e);
  static Json::Value toJSON(const CharacterSet& e);

  static const char* getName(CharacterSet::CharacterSetInternal e)
  {
     return (e>=0 && e<4) ? mHashTable[e].name : NULL;
  }

  static const CharacterSet::CharacterSetInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[4];
};

#endif
