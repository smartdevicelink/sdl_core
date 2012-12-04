#ifndef NSAPPLINKRPCV2_BUTTONCAPABILITIESMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_BUTTONCAPABILITIESMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/ButtonCapabilities.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
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
