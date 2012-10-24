#ifndef READDID_REQUESTMARSHALLER_INCLUDE
#define READDID_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/ReadDID_request.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct ReadDID_requestMarshaller
{
  static bool checkIntegrity(ReadDID_request& e);
  static bool checkIntegrityConst(const ReadDID_request& e);

  static bool fromString(const std::string& s,ReadDID_request& e);
  static const std::string toString(const ReadDID_request& e);

  static bool fromJSON(const Json::Value& s,ReadDID_request& e);
  static Json::Value toJSON(const ReadDID_request& e);
};
#endif
