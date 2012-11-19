#ifndef NSRPC2COMMUNICATION_BUTTONS_ONBUTTONPRESSMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_BUTTONS_ONBUTTONPRESSMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Buttons/OnButtonPress.h"

namespace NsRPC2Communication
{
  namespace Buttons
  {

    struct OnButtonPressMarshaller
    {
      static bool checkIntegrity(OnButtonPress& e);
      static bool checkIntegrityConst(const OnButtonPress& e);
    
      static bool fromString(const std::string& s,OnButtonPress& e);
      static const std::string toString(const OnButtonPress& e);
    
      static bool fromJSON(const Json::Value& s,OnButtonPress& e);
      static Json::Value toJSON(const OnButtonPress& e);
    };
  }
}

#endif
