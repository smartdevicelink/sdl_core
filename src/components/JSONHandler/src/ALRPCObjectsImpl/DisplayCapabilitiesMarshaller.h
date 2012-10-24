#ifndef DISPLAYCAPABILITIESMARSHALLER_INCLUDE
#define DISPLAYCAPABILITIESMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/DisplayCapabilities.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/


struct DisplayCapabilitiesMarshaller
{
  static bool checkIntegrity(DisplayCapabilities& e);
  static bool checkIntegrityConst(const DisplayCapabilities& e);

  static bool fromString(const std::string& s,DisplayCapabilities& e);
  static const std::string toString(const DisplayCapabilities& e);

  static bool fromJSON(const Json::Value& s,DisplayCapabilities& e);
  static Json::Value toJSON(const DisplayCapabilities& e);
};
#endif
