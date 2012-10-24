#ifndef TIRESTATUSMARSHALLER_INCLUDE
#define TIRESTATUSMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>

#include "../../include/JSONHandler/ALRPCObjects/TireStatus.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


struct TireStatusMarshaller
{
  static bool checkIntegrity(TireStatus& e);
  static bool checkIntegrityConst(const TireStatus& e);

  static bool fromString(const std::string& s,TireStatus& e);
  static const std::string toString(const TireStatus& e);

  static bool fromJSON(const Json::Value& s,TireStatus& e);
  static Json::Value toJSON(const TireStatus& e);
};
#endif
