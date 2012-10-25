#ifndef BUTTONCAPABILITIESMARSHALLER_INCLUDE
#define BUTTONCAPABILITIESMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "../../include/JSONHandler/ALRPCObjects/ButtonCapabilities.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
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
