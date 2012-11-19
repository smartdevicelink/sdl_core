#ifndef NSAPPLINKRPC_ONTBTCLIENTSTATEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_ONTBTCLIENTSTATEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/OnTBTClientState.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Mon Nov 19 06:40:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/

namespace NsAppLinkRPC
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

#endif
