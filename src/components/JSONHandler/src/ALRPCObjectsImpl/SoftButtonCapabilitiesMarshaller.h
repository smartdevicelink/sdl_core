#ifndef SOFTBUTTONCAPABILITIESMARSHALLER_INCLUDE
#define SOFTBUTTONCAPABILITIESMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/SoftButtonCapabilities.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct SoftButtonCapabilitiesMarshaller
{
  static bool checkIntegrity(SoftButtonCapabilities& e);
  static bool checkIntegrityConst(const SoftButtonCapabilities& e);

  static bool fromString(const std::string& s,SoftButtonCapabilities& e);
  static const std::string toString(const SoftButtonCapabilities& e);

  static bool fromJSON(const Json::Value& s,SoftButtonCapabilities& e);
  static Json::Value toJSON(const SoftButtonCapabilities& e);
};
#endif
