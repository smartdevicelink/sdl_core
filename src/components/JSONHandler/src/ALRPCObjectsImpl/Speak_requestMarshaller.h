#ifndef SPEAK_REQUESTMARSHALLER_INCLUDE
#define SPEAK_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/Speak_request.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct Speak_requestMarshaller
{
  static bool checkIntegrity(Speak_request& e);
  static bool checkIntegrityConst(const Speak_request& e);

  static bool fromString(const std::string& s,Speak_request& e);
  static const std::string toString(const Speak_request& e);

  static bool fromJSON(const Json::Value& s,Speak_request& e);
  static Json::Value toJSON(const Speak_request& e);
};
#endif
