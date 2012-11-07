#ifndef RPC2COMMUNICATION_BUTTONS_GETCAPABILITIESMARSHALLER_INCLUDE
#define RPC2COMMUNICATION_BUTTONS_GETCAPABILITIESMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/RPC2Objects/Buttons/GetCapabilities.h"

namespace RPC2Communication
{
  namespace Buttons
  {

    struct GetCapabilitiesMarshaller
    {
      static bool checkIntegrity(GetCapabilities& e);
      static bool checkIntegrityConst(const GetCapabilities& e);
    
      static bool fromString(const std::string& s,GetCapabilities& e);
      static const std::string toString(const GetCapabilities& e);
    
      static bool fromJSON(const Json::Value& s,GetCapabilities& e);
      static Json::Value toJSON(const GetCapabilities& e);
    };
  }
}

#endif
