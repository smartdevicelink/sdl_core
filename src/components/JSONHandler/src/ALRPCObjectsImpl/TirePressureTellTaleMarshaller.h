#ifndef TIREPRESSURETELLTALEMARSHALLER_INCLUDE
#define TIREPRESSURETELLTALEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/TirePressureTellTale.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


//! marshalling class for TirePressureTellTale

class TirePressureTellTaleMarshaller
{
public:

  static std::string toName(const TirePressureTellTale& e) 	{ return getName(e.mInternal) ?: ""; }

  static bool fromName(TirePressureTellTale& e,const std::string& s)
  { 
    return (e.mInternal=getIndex(s.c_str()))!=TirePressureTellTale::INVALID_ENUM;
  }

  static bool checkIntegrity(TirePressureTellTale& e)		{ return e.mInternal!=TirePressureTellTale::INVALID_ENUM; } 
  static bool checkIntegrityConst(const TirePressureTellTale& e)	{ return e.mInternal!=TirePressureTellTale::INVALID_ENUM; } 

  static bool fromString(const std::string& s,TirePressureTellTale& e);
  static const std::string toString(const TirePressureTellTale& e);

  static bool fromJSON(const Json::Value& s,TirePressureTellTale& e);
  static Json::Value toJSON(const TirePressureTellTale& e);

  static const char* getName(TirePressureTellTale::TirePressureTellTaleInternal e)
  {
     return (e>=0 && e<3) ? mHashTable[e].name : NULL;
  }

  static const TirePressureTellTale::TirePressureTellTaleInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[3];
};

#endif
