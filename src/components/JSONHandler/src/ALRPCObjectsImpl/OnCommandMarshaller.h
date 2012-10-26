#ifndef ONCOMMANDMARSHALLER_INCLUDE
#define ONCOMMANDMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "../../include/JSONHandler/ALRPCObjects/OnCommand.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/


struct OnCommandMarshaller
{
  static bool checkIntegrity(OnCommand& e);
  static bool checkIntegrityConst(const OnCommand& e);

  static bool fromString(const std::string& s,OnCommand& e);
  static const std::string toString(const OnCommand& e);

  static bool fromJSON(const Json::Value& s,OnCommand& e);
  static Json::Value toJSON(const OnCommand& e);
};
#endif
