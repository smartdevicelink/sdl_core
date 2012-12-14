#ifndef NSAPPLINKRPCV2_SOFTBUTTONCAPABILITIESMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_SOFTBUTTONCAPABILITIESMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/SoftButtonCapabilities.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
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
