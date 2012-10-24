#ifndef CHOICEMARSHALLER_INCLUDE
#define CHOICEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/Choice.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
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
