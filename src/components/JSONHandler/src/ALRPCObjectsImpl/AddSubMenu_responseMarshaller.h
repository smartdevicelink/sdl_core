#ifndef ADDSUBMENU_RESPONSEMARSHALLER_INCLUDE
#define ADDSUBMENU_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/AddSubMenu_response.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct AddSubMenu_responseMarshaller
{
  static bool checkIntegrity(AddSubMenu_response& e);
  static bool checkIntegrityConst(const AddSubMenu_response& e);

  static bool fromString(const std::string& s,AddSubMenu_response& e);
  static const std::string toString(const AddSubMenu_response& e);

  static bool fromJSON(const Json::Value& s,AddSubMenu_response& e);
  static Json::Value toJSON(const AddSubMenu_response& e);
};
#endif
