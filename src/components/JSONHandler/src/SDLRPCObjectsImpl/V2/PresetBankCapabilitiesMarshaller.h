//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_PRESETBANKCAPABILITIESMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_PRESETBANKCAPABILITIESMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/PresetBankCapabilities.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPCV2
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
