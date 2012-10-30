#ifndef SYNCMSGVERSIONMARSHALLER_INCLUDE
#define SYNCMSGVERSIONMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "../../include/JSONHandler/ALRPCObjects/SyncMsgVersion.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Oct 30 08:29:32 2012
  source stamp	Thu Oct 25 06:49:27 2012
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
