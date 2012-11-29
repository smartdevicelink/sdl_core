#ifndef NSAPPLINKRPC_SOFTBUTTONCAPABILITIESMARSHALLER_INCLUDE
#define NSAPPLINKRPC_SOFTBUTTONCAPABILITIESMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/SoftButtonCapabilities.h"


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

  struct SoftButtonCapabilitiesMarshaller
  {
    static bool checkIntegrity(SoftButtonCapabilities& e);
    static bool checkIntegrityConst(const SoftButtonCapabilities& e);
  
    static bool fromString(const std::string& s,SoftButtonCapabilities& e);
    static const std::string toString(const SoftButtonCapabilities& e);
  
    static bool fromJSON(const Json::Value& s,SoftButtonCapabilities& e);
    static Json::Value toJSON(const SoftButtonCapabilities& e);
  };
}

#endif
