#ifndef NSAPPLINKRPC_DISPLAYCAPABILITIESMARSHALLER_INCLUDE
#define NSAPPLINKRPC_DISPLAYCAPABILITIESMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V1/DisplayCapabilities.h"


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

  struct DisplayCapabilitiesMarshaller
  {
    static bool checkIntegrity(DisplayCapabilities& e);
    static bool checkIntegrityConst(const DisplayCapabilities& e);
  
    static bool fromString(const std::string& s,DisplayCapabilities& e);
    static const std::string toString(const DisplayCapabilities& e);
  
    static bool fromJSON(const Json::Value& s,DisplayCapabilities& e);
    static Json::Value toJSON(const DisplayCapabilities& e);
  };
}

#endif
