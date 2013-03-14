#ifndef NSAPPLINKRPCV2_SOFTBUTTONCAPABILITIESMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_SOFTBUTTONCAPABILITIESMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/SoftButtonCapabilities.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsSmartDeviceLinkRPCV2
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
