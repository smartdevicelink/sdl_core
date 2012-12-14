#ifndef NSRPC2COMMUNICATION_NAVIGATION_ONTBTCLIENTSTATEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_NAVIGATION_ONTBTCLIENTSTATEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Navigation/OnTBTClientState.h"

namespace NsRPC2Communication
{
  namespace Navigation
  {

    struct OnTBTClientStateMarshaller
    {
      static bool checkIntegrity(OnTBTClientState& e);
      static bool checkIntegrityConst(const OnTBTClientState& e);
    
      static bool fromString(const std::string& s,OnTBTClientState& e);
      static const std::string toString(const OnTBTClientState& e);
    
      static bool fromJSON(const Json::Value& s,OnTBTClientState& e);
      static Json::Value toJSON(const OnTBTClientState& e);
    };
  }
}

#endif
