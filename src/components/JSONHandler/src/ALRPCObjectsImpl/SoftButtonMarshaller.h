#ifndef SOFTBUTTONMARSHALLER_INCLUDE
#define SOFTBUTTONMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/SoftButton.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct SoftButtonMarshaller
{
  static bool checkIntegrity(SoftButton& e);
  static bool checkIntegrityConst(const SoftButton& e);

  static bool fromString(const std::string& s,SoftButton& e);
  static const std::string toString(const SoftButton& e);

  static bool fromJSON(const Json::Value& s,SoftButton& e);
  static Json::Value toJSON(const SoftButton& e);
};
#endif
