#ifndef DELETESUBMENU_RESPONSEMARSHALLER_INCLUDE
#define DELETESUBMENU_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>

#include "../../include/JSONHandler/ALRPCObjects/DeleteSubMenu_response.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


struct DeleteSubMenu_responseMarshaller
{
  static bool checkIntegrity(DeleteSubMenu_response& e);
  static bool checkIntegrityConst(const DeleteSubMenu_response& e);

  static bool fromString(const std::string& s,DeleteSubMenu_response& e);
  static const std::string toString(const DeleteSubMenu_response& e);

  static bool fromJSON(const Json::Value& s,DeleteSubMenu_response& e);
  static Json::Value toJSON(const DeleteSubMenu_response& e);
};
#endif
