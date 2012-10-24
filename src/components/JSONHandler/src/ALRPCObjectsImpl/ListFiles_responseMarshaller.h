#ifndef LISTFILES_RESPONSEMARSHALLER_INCLUDE
#define LISTFILES_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/ListFiles_response.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct ListFiles_responseMarshaller
{
  static bool checkIntegrity(ListFiles_response& e);
  static bool checkIntegrityConst(const ListFiles_response& e);

  static bool fromString(const std::string& s,ListFiles_response& e);
  static const std::string toString(const ListFiles_response& e);

  static bool fromJSON(const Json::Value& s,ListFiles_response& e);
  static Json::Value toJSON(const ListFiles_response& e);
};
#endif
