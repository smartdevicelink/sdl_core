#ifndef SYNCMSGVERSIONMARSHALLER_INCLUDE
#define SYNCMSGVERSIONMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/SyncMsgVersion.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct SyncMsgVersionMarshaller
{
  static bool checkIntegrity(SyncMsgVersion& e);
  static bool checkIntegrityConst(const SyncMsgVersion& e);

  static bool fromString(const std::string& s,SyncMsgVersion& e);
  static const std::string toString(const SyncMsgVersion& e);

  static bool fromJSON(const Json::Value& s,SyncMsgVersion& e);
  static Json::Value toJSON(const SyncMsgVersion& e);
};
#endif
