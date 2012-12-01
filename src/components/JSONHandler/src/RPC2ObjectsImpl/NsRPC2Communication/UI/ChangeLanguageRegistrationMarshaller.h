#ifndef NSRPC2COMMUNICATION_UI_CHANGELANGUAGEREGISTRATIONMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_CHANGELANGUAGEREGISTRATIONMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/ChangeLanguageRegistration.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct ChangeLanguageRegistrationMarshaller
    {
      static bool checkIntegrity(ChangeLanguageRegistration& e);
      static bool checkIntegrityConst(const ChangeLanguageRegistration& e);
    
      static bool fromString(const std::string& s,ChangeLanguageRegistration& e);
      static const std::string toString(const ChangeLanguageRegistration& e);
    
      static bool fromJSON(const Json::Value& s,ChangeLanguageRegistration& e);
      static Json::Value toJSON(const ChangeLanguageRegistration& e);
    };
  }
}

#endif
