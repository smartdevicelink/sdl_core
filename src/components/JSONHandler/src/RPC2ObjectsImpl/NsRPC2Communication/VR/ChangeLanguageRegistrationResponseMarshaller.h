#ifndef NSRPC2COMMUNICATION_VR_CHANGELANGUAGEREGISTRATIONRESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_VR_CHANGELANGUAGEREGISTRATIONRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VR/ChangeLanguageRegistrationResponse.h"

namespace NsRPC2Communication
{
  namespace VR
  {

    struct ChangeLanguageRegistrationResponseMarshaller
    {
      static bool checkIntegrity(ChangeLanguageRegistrationResponse& e);
      static bool checkIntegrityConst(const ChangeLanguageRegistrationResponse& e);
    
      static bool fromString(const std::string& s,ChangeLanguageRegistrationResponse& e);
      static const std::string toString(const ChangeLanguageRegistrationResponse& e);
    
      static bool fromJSON(const Json::Value& s,ChangeLanguageRegistrationResponse& e);
      static Json::Value toJSON(const ChangeLanguageRegistrationResponse& e);
    };
  }
}

#endif
