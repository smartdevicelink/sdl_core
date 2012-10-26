#ifndef SETGLOBALPROPERTIES_REQUESTMARSHALLER_INCLUDE
#define SETGLOBALPROPERTIES_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "../../include/JSONHandler/ALRPCObjects/SetGlobalProperties_request.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/


struct SetGlobalProperties_requestMarshaller
{
  static bool checkIntegrity(SetGlobalProperties_request& e);
  static bool checkIntegrityConst(const SetGlobalProperties_request& e);

  static bool fromString(const std::string& s,SetGlobalProperties_request& e);
  static const std::string toString(const SetGlobalProperties_request& e);

  static bool fromJSON(const Json::Value& s,SetGlobalProperties_request& e);
  static Json::Value toJSON(const SetGlobalProperties_request& e);
};
#endif
