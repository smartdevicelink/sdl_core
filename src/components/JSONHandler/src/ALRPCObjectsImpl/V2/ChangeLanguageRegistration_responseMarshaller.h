#ifndef NSAPPLINKRPCV2_CHANGELANGUAGEREGISTRATION_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_CHANGELANGUAGEREGISTRATION_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/ChangeLanguageRegistration_response.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  struct ChangeLanguageRegistration_responseMarshaller
  {
    static bool checkIntegrity(ChangeLanguageRegistration_response& e);
    static bool checkIntegrityConst(const ChangeLanguageRegistration_response& e);
  
    static bool fromString(const std::string& s,ChangeLanguageRegistration_response& e);
    static const std::string toString(const ChangeLanguageRegistration_response& e);
  
    static bool fromJSON(const Json::Value& s,ChangeLanguageRegistration_response& e);
    static Json::Value toJSON(const ChangeLanguageRegistration_response& e);
  };
}

#endif
