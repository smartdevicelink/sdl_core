#ifndef TIRESTATUSMARSHALLER_INCLUDE
#define TIRESTATUSMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/TireStatus.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
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
