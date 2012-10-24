#ifndef DELETESUBMENU_REQUESTMARSHALLER_INCLUDE
#define DELETESUBMENU_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/DeleteSubMenu_request.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct DeleteSubMenu_requestMarshaller
{
  static bool checkIntegrity(DeleteSubMenu_request& e);
  static bool checkIntegrityConst(const DeleteSubMenu_request& e);

  static bool fromString(const std::string& s,DeleteSubMenu_request& e);
  static const std::string toString(const DeleteSubMenu_request& e);

  static bool fromJSON(const Json::Value& s,DeleteSubMenu_request& e);
  static Json::Value toJSON(const DeleteSubMenu_request& e);
};
#endif
