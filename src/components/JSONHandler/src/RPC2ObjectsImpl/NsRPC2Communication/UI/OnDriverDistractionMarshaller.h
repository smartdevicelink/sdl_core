#ifndef NSRPC2COMMUNICATION_UI_ONDRIVERDISTRACTIONMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_ONDRIVERDISTRACTIONMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnDriverDistraction.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct OnDriverDistractionMarshaller
    {
      static bool checkIntegrity(OnDriverDistraction& e);
      static bool checkIntegrityConst(const OnDriverDistraction& e);
    
      static bool fromString(const std::string& s,OnDriverDistraction& e);
      static const std::string toString(const OnDriverDistraction& e);
    
      static bool fromJSON(const Json::Value& s,OnDriverDistraction& e);
      static Json::Value toJSON(const OnDriverDistraction& e);
    };
  }
}

#endif
