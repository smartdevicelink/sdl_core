#ifndef NSRPC2COMMUNICATION_UI_DIALNUMBERRESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_DIALNUMBERRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/DialNumberResponse.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct DialNumberResponseMarshaller
    {
      static bool checkIntegrity(DialNumberResponse& e);
      static bool checkIntegrityConst(const DialNumberResponse& e);
    
      static bool fromString(const std::string& s,DialNumberResponse& e);
      static const std::string toString(const DialNumberResponse& e);
    
      static bool fromJSON(const Json::Value& s,DialNumberResponse& e);
      static Json::Value toJSON(const DialNumberResponse& e);
    };
  }
}

#endif
