#ifndef DTCMARSHALLER_INCLUDE
#define DTCMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/DTC.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct DTCMarshaller
{
  static bool checkIntegrity(DTC& e);
  static bool checkIntegrityConst(const DTC& e);

  static bool fromString(const std::string& s,DTC& e);
  static const std::string toString(const DTC& e);

  static bool fromJSON(const Json::Value& s,DTC& e);
  static Json::Value toJSON(const DTC& e);
};
#endif
