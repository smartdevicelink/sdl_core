#ifndef GLOBALPROPERTYMARSHALLER_INCLUDE
#define GLOBALPROPERTYMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/GlobalProperty.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/


//! marshalling class for GlobalProperty

class GlobalPropertyMarshaller
{
public:

  static std::string toName(const GlobalProperty& e) 	{ return getName(e.mInternal) ?: ""; }

  static bool fromName(GlobalProperty& e,const std::string& s)
  { 
    return (e.mInternal=getIndex(s.c_str()))!=GlobalProperty::INVALID_ENUM;
  }

  static bool checkIntegrity(GlobalProperty& e)		{ return e.mInternal!=GlobalProperty::INVALID_ENUM; } 
  static bool checkIntegrityConst(const GlobalProperty& e)	{ return e.mInternal!=GlobalProperty::INVALID_ENUM; } 

  static bool fromString(const std::string& s,GlobalProperty& e);
  static const std::string toString(const GlobalProperty& e);

  static bool fromJSON(const Json::Value& s,GlobalProperty& e);
  static Json::Value toJSON(const GlobalProperty& e);

  static const char* getName(GlobalProperty::GlobalPropertyInternal e)
  {
     return (e>=0 && e<2) ? mHashTable[e].name : NULL;
  }

  static const GlobalProperty::GlobalPropertyInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[2];
};

#endif
