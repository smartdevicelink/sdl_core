#ifndef CHOICEMARSHALLER_INCLUDE
#define CHOICEMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>

#include "../../include/JSONHandler/ALRPCObjects/Choice.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
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
