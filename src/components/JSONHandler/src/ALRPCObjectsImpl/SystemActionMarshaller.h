#ifndef SYSTEMACTIONMARSHALLER_INCLUDE
#define SYSTEMACTIONMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/SystemAction.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/


//! marshalling class for SystemAction

class SystemActionMarshaller
{
public:

  static std::string toName(const SystemAction& e) 	{ return getName(e.mInternal) ?: ""; }

  static bool fromName(SystemAction& e,const std::string& s)
  { 
    return (e.mInternal=getIndex(s.c_str()))!=SystemAction::INVALID_ENUM;
  }

  static bool checkIntegrity(SystemAction& e)		{ return e.mInternal!=SystemAction::INVALID_ENUM; } 
  static bool checkIntegrityConst(const SystemAction& e)	{ return e.mInternal!=SystemAction::INVALID_ENUM; } 

  static bool fromString(const std::string& s,SystemAction& e);
  static const std::string toString(const SystemAction& e);

  static bool fromJSON(const Json::Value& s,SystemAction& e);
  static Json::Value toJSON(const SystemAction& e);

  static const char* getName(SystemAction::SystemActionInternal e)
  {
     return (e>=0 && e<3) ? mHashTable[e].name : NULL;
  }

  static const SystemAction::SystemActionInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[3];
};

#endif
