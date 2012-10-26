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
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
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
