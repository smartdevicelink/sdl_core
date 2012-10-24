#ifndef PERFORMINTERACTION_RESPONSEMARSHALLER_INCLUDE
#define PERFORMINTERACTION_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>

#include "../../include/JSONHandler/ALRPCObjects/PerformInteraction_response.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


struct PerformInteraction_responseMarshaller
{
  static bool checkIntegrity(PerformInteraction_response& e);
  static bool checkIntegrityConst(const PerformInteraction_response& e);

  static bool fromString(const std::string& s,PerformInteraction_response& e);
  static const std::string toString(const PerformInteraction_response& e);

  static bool fromJSON(const Json::Value& s,PerformInteraction_response& e);
  static Json::Value toJSON(const PerformInteraction_response& e);
};
#endif
