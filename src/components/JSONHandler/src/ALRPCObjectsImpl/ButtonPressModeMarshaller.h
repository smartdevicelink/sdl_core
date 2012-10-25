#ifndef BUTTONPRESSMODEMARSHALLER_INCLUDE
#define BUTTONPRESSMODEMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/ButtonPressMode.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
  author	robok0der
*/


//! marshalling class for ButtonPressMode

class ButtonPressModeMarshaller
{
public:

  static std::string toName(const ButtonPressMode& e) 	{ return getName(e.mInternal) ?: ""; }

  static bool fromName(ButtonPressMode& e,const std::string& s)
  { 
    return (e.mInternal=getIndex(s.c_str()))!=ButtonPressMode::INVALID_ENUM;
  }

  static bool checkIntegrity(ButtonPressMode& e)		{ return e.mInternal!=ButtonPressMode::INVALID_ENUM; } 
  static bool checkIntegrityConst(const ButtonPressMode& e)	{ return e.mInternal!=ButtonPressMode::INVALID_ENUM; } 

  static bool fromString(const std::string& s,ButtonPressMode& e);
  static const std::string toString(const ButtonPressMode& e);

  static bool fromJSON(const Json::Value& s,ButtonPressMode& e);
  static Json::Value toJSON(const ButtonPressMode& e);

  static const char* getName(ButtonPressMode::ButtonPressModeInternal e)
  {
     return (e>=0 && e<2) ? mHashTable[e].name : NULL;
  }

  static const ButtonPressMode::ButtonPressModeInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[2];
};

#endif
