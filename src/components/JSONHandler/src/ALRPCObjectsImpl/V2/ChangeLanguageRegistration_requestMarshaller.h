#ifndef NSAPPLINKRPCV2_CHANGELANGUAGEREGISTRATION_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_CHANGELANGUAGEREGISTRATION_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/ChangeLanguageRegistration_request.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  struct ChangeLanguageRegistration_requestMarshaller
  {
    static bool checkIntegrity(ChangeLanguageRegistration_request& e);
    static bool checkIntegrityConst(const ChangeLanguageRegistration_request& e);
  
    static bool fromString(const std::string& s,ChangeLanguageRegistration_request& e);
    static const std::string toString(const ChangeLanguageRegistration_request& e);
  
    static bool fromJSON(const Json::Value& s,ChangeLanguageRegistration_request& e);
    static Json::Value toJSON(const ChangeLanguageRegistration_request& e);
  };
}

#endif
