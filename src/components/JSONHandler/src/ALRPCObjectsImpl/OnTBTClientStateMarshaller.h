#ifndef NSAPPLINKRPC_ONTBTCLIENTSTATEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_ONTBTCLIENTSTATEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/OnTBTClientState.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
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
