#ifndef TEXTFIELDMARSHALLER_INCLUDE
#define TEXTFIELDMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/TextField.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct TextFieldMarshaller
{
  static bool checkIntegrity(TextField& e);
  static bool checkIntegrityConst(const TextField& e);

  static bool fromString(const std::string& s,TextField& e);
  static const std::string toString(const TextField& e);

  static bool fromJSON(const Json::Value& s,TextField& e);
  static Json::Value toJSON(const TextField& e);
};
#endif
