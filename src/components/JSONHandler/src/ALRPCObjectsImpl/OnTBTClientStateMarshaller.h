#ifndef ONTBTCLIENTSTATEMARSHALLER_INCLUDE
#define ONTBTCLIENTSTATEMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>

#include "../../include/JSONHandler/ALRPCObjects/OnTBTClientState.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


struct OnTBTClientStateMarshaller
{
  static bool checkIntegrity(OnTBTClientState& e);
  static bool checkIntegrityConst(const OnTBTClientState& e);

  static bool fromString(const std::string& s,OnTBTClientState& e);
  static const std::string toString(const OnTBTClientState& e);

  static bool fromJSON(const Json::Value& s,OnTBTClientState& e);
  static Json::Value toJSON(const OnTBTClientState& e);
};
#endif
