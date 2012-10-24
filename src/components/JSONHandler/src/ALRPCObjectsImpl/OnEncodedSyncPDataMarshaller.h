#ifndef ONENCODEDSYNCPDATAMARSHALLER_INCLUDE
#define ONENCODEDSYNCPDATAMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>

#include "../../include/JSONHandler/ALRPCObjects/OnEncodedSyncPData.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
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
