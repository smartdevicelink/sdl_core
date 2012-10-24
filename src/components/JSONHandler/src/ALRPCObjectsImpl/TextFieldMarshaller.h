#ifndef TEXTFIELDMARSHALLER_INCLUDE
#define TEXTFIELDMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>

#include "../../include/JSONHandler/ALRPCObjects/TextField.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
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
