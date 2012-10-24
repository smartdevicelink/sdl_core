#ifndef CHANGELANGUAGEREGISTRATION_REQUESTMARSHALLER_INCLUDE
#define CHANGELANGUAGEREGISTRATION_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/ChangeLanguageRegistration_request.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct ChangeLanguageRegistration_requestMarshaller
{
  static bool checkIntegrity(ChangeLanguageRegistration_request& e);
  static bool checkIntegrityConst(const ChangeLanguageRegistration_request& e);

  static bool fromString(const std::string& s,ChangeLanguageRegistration_request& e);
  static const std::string toString(const ChangeLanguageRegistration_request& e);

  static bool fromJSON(const Json::Value& s,ChangeLanguageRegistration_request& e);
  static Json::Value toJSON(const ChangeLanguageRegistration_request& e);
};
#endif
