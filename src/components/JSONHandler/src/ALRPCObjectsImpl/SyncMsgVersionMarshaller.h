#ifndef SYNCMSGVERSIONMARSHALLER_INCLUDE
#define SYNCMSGVERSIONMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>

#include "../../include/JSONHandler/ALRPCObjects/SyncMsgVersion.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
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
