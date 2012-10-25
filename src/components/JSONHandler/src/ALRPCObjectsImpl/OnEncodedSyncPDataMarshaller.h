#ifndef ONENCODEDSYNCPDATAMARSHALLER_INCLUDE
#define ONENCODEDSYNCPDATAMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "../../include/JSONHandler/ALRPCObjects/OnEncodedSyncPData.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
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
