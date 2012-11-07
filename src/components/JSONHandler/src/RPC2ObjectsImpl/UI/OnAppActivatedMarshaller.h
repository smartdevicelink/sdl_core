#ifndef RPC2COMMUNICATION_UI_ONAPPACTIVATEDMARSHALLER_INCLUDE
#define RPC2COMMUNICATION_UI_ONAPPACTIVATEDMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/RPC2Objects/UI/OnAppActivated.h"

namespace RPC2Communication
{
  namespace UI
  {

    struct OnAppActivatedMarshaller
    {
      static bool checkIntegrity(OnAppActivated& e);
      static bool checkIntegrityConst(const OnAppActivated& e);
    
      static bool fromString(const std::string& s,OnAppActivated& e);
      static const std::string toString(const OnAppActivated& e);
    
      static bool fromJSON(const Json::Value& s,OnAppActivated& e);
      static Json::Value toJSON(const OnAppActivated& e);
    };
  }
}

#endif
