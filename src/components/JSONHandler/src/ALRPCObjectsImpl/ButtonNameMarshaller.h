#ifndef BUTTONNAMEMARSHALLER_INCLUDE
#define BUTTONNAMEMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/ButtonName.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


//! marshalling class for ButtonName

class ButtonNameMarshaller
{
public:

  static std::string toName(const ButtonName& e) 	{ return getName(e.mInternal) ?: ""; }

  static bool fromName(ButtonName& e,const std::string& s)
  { 
    return (e.mInternal=getIndex(s.c_str()))!=ButtonName::INVALID_ENUM;
  }

  static bool checkIntegrity(ButtonName& e)		{ return e.mInternal!=ButtonName::INVALID_ENUM; } 
  static bool checkIntegrityConst(const ButtonName& e)	{ return e.mInternal!=ButtonName::INVALID_ENUM; } 

  static bool fromString(const std::string& s,ButtonName& e);
  static const std::string toString(const ButtonName& e);

  static bool fromJSON(const Json::Value& s,ButtonName& e);
  static Json::Value toJSON(const ButtonName& e);

  static const char* getName(ButtonName::ButtonNameInternal e)
  {
     return (e>=0 && e<15) ? mHashTable[e].name : NULL;
  }

  static const ButtonName::ButtonNameInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[15];
};

#endif
