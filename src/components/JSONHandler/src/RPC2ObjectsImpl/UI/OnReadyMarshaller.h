#ifndef ONREADYMARSHALLER
#define ONREADYMARSHALLER

#include "JSONHandler/RPC2Objects/UI/OnReady.h"
#include <json/json.h>
#include <string>

namespace RPC2Communication
{
  namespace UI
  {
    class OnReadyMarshaller
    {
    public:
      static bool checkIntegrity(OnReady& e);
      static bool checkIntegrityConst(const OnReady& e);
    
      static bool fromString(const std::string& s,OnReady& e);
      static const std::string toString(const OnReady& e);
    
      static bool fromJSON(const Json::Value& s,OnReady& e);
      static Json::Value toJSON(const OnReady& e);
    };
  }
}

#endif