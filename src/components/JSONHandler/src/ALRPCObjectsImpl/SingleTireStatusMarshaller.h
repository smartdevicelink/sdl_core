#ifndef SINGLETIRESTATUSMARSHALLER_INCLUDE
#define SINGLETIRESTATUSMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "../../include/JSONHandler/ALRPCObjects/SingleTireStatus.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
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
