#ifndef DELETESUBMENU_REQUESTMARSHALLER_INCLUDE
#define DELETESUBMENU_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "../../include/JSONHandler/ALRPCObjects/DeleteSubMenu_request.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
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
