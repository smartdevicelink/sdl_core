#ifndef BUTTONCAPABILITIESMARSHALLER_INCLUDE
#define BUTTONCAPABILITIESMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/ButtonCapabilities.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/


struct ButtonCapabilitiesMarshaller
{
  static bool checkIntegrity(ButtonCapabilities& e);
  static bool checkIntegrityConst(const ButtonCapabilities& e);

  static bool fromString(const std::string& s,ButtonCapabilities& e);
  static const std::string toString(const ButtonCapabilities& e);

  static bool fromJSON(const Json::Value& s,ButtonCapabilities& e);
  static Json::Value toJSON(const ButtonCapabilities& e);
};
#endif
