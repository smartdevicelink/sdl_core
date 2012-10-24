#ifndef PRESETBANKCAPABILITIESMARSHALLER_INCLUDE
#define PRESETBANKCAPABILITIESMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/PresetBankCapabilities.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/


struct PresetBankCapabilitiesMarshaller
{
  static bool checkIntegrity(PresetBankCapabilities& e);
  static bool checkIntegrityConst(const PresetBankCapabilities& e);

  static bool fromString(const std::string& s,PresetBankCapabilities& e);
  static const std::string toString(const PresetBankCapabilities& e);

  static bool fromJSON(const Json::Value& s,PresetBankCapabilities& e);
  static Json::Value toJSON(const PresetBankCapabilities& e);
};
#endif
