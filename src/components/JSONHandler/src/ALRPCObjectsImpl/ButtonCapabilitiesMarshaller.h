#ifndef APPLINKRPC_BUTTONCAPABILITIESMARSHALLER_INCLUDE
#define APPLINKRPC_BUTTONCAPABILITIESMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/ButtonCapabilities.h"


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

  struct ButtonCapabilitiesMarshaller
  {
    static bool checkIntegrity(ButtonCapabilities& e);
    static bool checkIntegrityConst(const ButtonCapabilities& e);
  
    static bool fromString(const std::string& s,ButtonCapabilities& e);
    static const std::string toString(const ButtonCapabilities& e);
  
    static bool fromJSON(const Json::Value& s,ButtonCapabilities& e);
    static Json::Value toJSON(const ButtonCapabilities& e);
  };
}

#endif
