#ifndef UPDATETURNLIST_REQUESTMARSHALLER_INCLUDE
#define UPDATETURNLIST_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/UpdateTurnList_request.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct UpdateTurnList_requestMarshaller
{
  static bool checkIntegrity(UpdateTurnList_request& e);
  static bool checkIntegrityConst(const UpdateTurnList_request& e);

  static bool fromString(const std::string& s,UpdateTurnList_request& e);
  static const std::string toString(const UpdateTurnList_request& e);

  static bool fromJSON(const Json::Value& s,UpdateTurnList_request& e);
  static Json::Value toJSON(const UpdateTurnList_request& e);
};
#endif
