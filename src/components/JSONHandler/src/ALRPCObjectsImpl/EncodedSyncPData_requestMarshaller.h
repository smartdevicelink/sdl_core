#ifndef ENCODEDSYNCPDATA_REQUESTMARSHALLER_INCLUDE
#define ENCODEDSYNCPDATA_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "../../include/JSONHandler/ALRPCObjects/EncodedSyncPData_request.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
  author	robok0der
*/


struct EncodedSyncPData_requestMarshaller
{
  static bool checkIntegrity(EncodedSyncPData_request& e);
  static bool checkIntegrityConst(const EncodedSyncPData_request& e);

  static bool fromString(const std::string& s,EncodedSyncPData_request& e);
  static const std::string toString(const EncodedSyncPData_request& e);

  static bool fromJSON(const Json::Value& s,EncodedSyncPData_request& e);
  static Json::Value toJSON(const EncodedSyncPData_request& e);
};
#endif
