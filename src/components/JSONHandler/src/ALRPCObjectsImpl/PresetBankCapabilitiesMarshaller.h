#ifndef NSAPPLINKRPC_PRESETBANKCAPABILITIESMARSHALLER_INCLUDE
#define NSAPPLINKRPC_PRESETBANKCAPABILITIESMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/PresetBankCapabilities.h"


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
