#ifndef BUTTONPRESSMODEMARSHALLER_INCLUDE
#define BUTTONPRESSMODEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/ButtonPressMode.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
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
