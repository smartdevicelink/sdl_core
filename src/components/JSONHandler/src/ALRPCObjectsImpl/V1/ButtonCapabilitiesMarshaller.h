#ifndef NSAPPLINKRPC_BUTTONCAPABILITIESMARSHALLER_INCLUDE
#define NSAPPLINKRPC_BUTTONCAPABILITIESMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V1/ButtonCapabilities.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Dec 13 13:37:09 2012
  source stamp	Thu Dec 13 13:33:23 2012
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
