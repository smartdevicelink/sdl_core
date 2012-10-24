#ifndef SINGLETIRESTATUSMARSHALLER_INCLUDE
#define SINGLETIRESTATUSMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/SingleTireStatus.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct SingleTireStatusMarshaller
{
  static bool checkIntegrity(SingleTireStatus& e);
  static bool checkIntegrityConst(const SingleTireStatus& e);

  static bool fromString(const std::string& s,SingleTireStatus& e);
  static const std::string toString(const SingleTireStatus& e);

  static bool fromJSON(const Json::Value& s,SingleTireStatus& e);
  static Json::Value toJSON(const SingleTireStatus& e);
};
#endif
