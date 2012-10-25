#ifndef DISPLAYCAPABILITIESMARSHALLER_INCLUDE
#define DISPLAYCAPABILITIESMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "../../include/JSONHandler/ALRPCObjects/DisplayCapabilities.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
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
