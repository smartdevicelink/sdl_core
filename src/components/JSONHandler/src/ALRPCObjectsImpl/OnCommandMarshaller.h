#ifndef APPLINKRPC_ONCOMMANDMARSHALLER_INCLUDE
#define APPLINKRPC_ONCOMMANDMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/OnCommand.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Nov  7 13:10:41 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/

namespace AppLinkRPC
{

  struct OnCommandMarshaller
  {
    static bool checkIntegrity(OnCommand& e);
    static bool checkIntegrityConst(const OnCommand& e);
  
    static bool fromString(const std::string& s,OnCommand& e);
    static const std::string toString(const OnCommand& e);
  
    static bool fromJSON(const Json::Value& s,OnCommand& e);
    static Json::Value toJSON(const OnCommand& e);
  };
}

#endif
