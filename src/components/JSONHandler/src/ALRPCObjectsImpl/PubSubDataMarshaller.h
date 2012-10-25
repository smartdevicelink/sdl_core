#ifndef PUBSUBDATAMARSHALLER_INCLUDE
#define PUBSUBDATAMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "../../include/JSONHandler/ALRPCObjects/PubSubData.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


struct PubSubDataMarshaller
{
  static bool checkIntegrity(PubSubData& e);
  static bool checkIntegrityConst(const PubSubData& e);

  static bool fromString(const std::string& s,PubSubData& e);
  static const std::string toString(const PubSubData& e);

  static bool fromJSON(const Json::Value& s,PubSubData& e);
  static Json::Value toJSON(const PubSubData& e);
};
#endif
