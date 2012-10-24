#ifndef DISPLAYTYPEMARSHALLER_INCLUDE
#define DISPLAYTYPEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/DisplayType.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/


//! marshalling class for DisplayType

class DisplayTypeMarshaller
{
public:

  static std::string toName(const DisplayType& e) 	{ return getName(e.mInternal) ?: ""; }

  static bool fromName(DisplayType& e,const std::string& s)
  { 
    return (e.mInternal=getIndex(s.c_str()))!=DisplayType::INVALID_ENUM;
  }

  static bool checkIntegrity(DisplayType& e)		{ return e.mInternal!=DisplayType::INVALID_ENUM; } 
  static bool checkIntegrityConst(const DisplayType& e)	{ return e.mInternal!=DisplayType::INVALID_ENUM; } 

  static bool fromString(const std::string& s,DisplayType& e);
  static const std::string toString(const DisplayType& e);

  static bool fromJSON(const Json::Value& s,DisplayType& e);
  static Json::Value toJSON(const DisplayType& e);

  static const char* getName(DisplayType::DisplayTypeInternal e)
  {
     return (e>=0 && e<9) ? mHashTable[e].name : NULL;
  }

  static const DisplayType::DisplayTypeInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[9];
};

#endif
