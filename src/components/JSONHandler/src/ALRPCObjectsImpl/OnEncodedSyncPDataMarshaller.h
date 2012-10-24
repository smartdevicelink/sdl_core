#ifndef ONENCODEDSYNCPDATAMARSHALLER_INCLUDE
#define ONENCODEDSYNCPDATAMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/OnEncodedSyncPData.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct OnEncodedSyncPDataMarshaller
{
  static bool checkIntegrity(OnEncodedSyncPData& e);
  static bool checkIntegrityConst(const OnEncodedSyncPData& e);

  static bool fromString(const std::string& s,OnEncodedSyncPData& e);
  static const std::string toString(const OnEncodedSyncPData& e);

  static bool fromJSON(const Json::Value& s,OnEncodedSyncPData& e);
  static Json::Value toJSON(const OnEncodedSyncPData& e);
};
#endif
