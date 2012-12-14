#ifndef NSAPPLINKRPCV2_PRESETBANKCAPABILITIESMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_PRESETBANKCAPABILITIESMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/PresetBankCapabilities.h"


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

  struct PresetBankCapabilitiesMarshaller
  {
    static bool checkIntegrity(PresetBankCapabilities& e);
    static bool checkIntegrityConst(const PresetBankCapabilities& e);
  
    static bool fromString(const std::string& s,PresetBankCapabilities& e);
    static const std::string toString(const PresetBankCapabilities& e);
  
    static bool fromJSON(const Json::Value& s,PresetBankCapabilities& e);
    static Json::Value toJSON(const PresetBankCapabilities& e);
  };
}

#endif
