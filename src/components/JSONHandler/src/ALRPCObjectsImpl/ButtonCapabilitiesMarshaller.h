#ifndef NSAPPLINKRPC_BUTTONCAPABILITIESMARSHALLER_INCLUDE
#define NSAPPLINKRPC_BUTTONCAPABILITIESMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/ButtonCapabilities.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Mon Nov 19 10:37:06 2012
  source stamp	Mon Nov 19 10:35:56 2012
  author	robok0der
*/

namespace NsAppLinkRPC
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
