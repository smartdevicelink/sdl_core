#ifndef GLOBALPROPERTYMARSHALLER_INCLUDE
#define GLOBALPROPERTYMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/GlobalProperty.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
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
     return (e>=0 && e<4) ? mHashTable[e].name : NULL;
  }

  static const GlobalProperty::GlobalPropertyInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[4];
};

#endif
