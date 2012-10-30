#ifndef PERFORMINTERACTION_RESPONSEMARSHALLER_INCLUDE
#define PERFORMINTERACTION_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "../../include/JSONHandler/ALRPCObjects/PerformInteraction_response.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Oct 30 08:29:32 2012
  source stamp	Thu Oct 25 06:49:27 2012
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
