#ifndef ONHMISTATUSMARSHALLER_INCLUDE
#define ONHMISTATUSMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "../../include/JSONHandler/ALRPCObjects/OnHMIStatus.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/


struct OnHMIStatusMarshaller
{
  static bool checkIntegrity(OnHMIStatus& e);
  static bool checkIntegrityConst(const OnHMIStatus& e);

  static bool fromString(const std::string& s,OnHMIStatus& e);
  static const std::string toString(const OnHMIStatus& e);

  static bool fromJSON(const Json::Value& s,OnHMIStatus& e);
  static Json::Value toJSON(const OnHMIStatus& e);
};
#endif
