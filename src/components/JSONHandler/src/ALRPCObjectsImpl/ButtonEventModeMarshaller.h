#ifndef BUTTONEVENTMODEMARSHALLER_INCLUDE
#define BUTTONEVENTMODEMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/ButtonEventMode.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/


//! marshalling class for ButtonEventMode

class ButtonEventModeMarshaller
{
public:

  static std::string toName(const ButtonEventMode& e) 	{ return getName(e.mInternal) ?: ""; }

  static bool fromName(ButtonEventMode& e,const std::string& s)
  { 
    return (e.mInternal=getIndex(s.c_str()))!=ButtonEventMode::INVALID_ENUM;
  }

  static bool checkIntegrity(ButtonEventMode& e)		{ return e.mInternal!=ButtonEventMode::INVALID_ENUM; } 
  static bool checkIntegrityConst(const ButtonEventMode& e)	{ return e.mInternal!=ButtonEventMode::INVALID_ENUM; } 

  static bool fromString(const std::string& s,ButtonEventMode& e);
  static const std::string toString(const ButtonEventMode& e);

  static bool fromJSON(const Json::Value& s,ButtonEventMode& e);
  static Json::Value toJSON(const ButtonEventMode& e);

  static const char* getName(ButtonEventMode::ButtonEventModeInternal e)
  {
     return (e>=0 && e<2) ? mHashTable[e].name : NULL;
  }

  static const ButtonEventMode::ButtonEventModeInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[2];
};

#endif
