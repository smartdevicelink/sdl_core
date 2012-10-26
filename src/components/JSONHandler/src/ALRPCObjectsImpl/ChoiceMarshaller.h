#ifndef CHOICEMARSHALLER_INCLUDE
#define CHOICEMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "../../include/JSONHandler/ALRPCObjects/Choice.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/


struct ChoiceMarshaller
{
  static bool checkIntegrity(Choice& e);
  static bool checkIntegrityConst(const Choice& e);

  static bool fromString(const std::string& s,Choice& e);
  static const std::string toString(const Choice& e);

  static bool fromJSON(const Json::Value& s,Choice& e);
  static Json::Value toJSON(const Choice& e);
};
#endif
