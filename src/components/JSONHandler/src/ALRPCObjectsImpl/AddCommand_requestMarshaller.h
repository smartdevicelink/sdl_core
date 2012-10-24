#ifndef ADDCOMMAND_REQUESTMARSHALLER_INCLUDE
#define ADDCOMMAND_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>

#include "../../include/JSONHandler/ALRPCObjects/AddCommand_request.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


struct AddCommand_requestMarshaller
{
  static bool checkIntegrity(AddCommand_request& e);
  static bool checkIntegrityConst(const AddCommand_request& e);

  static bool fromString(const std::string& s,AddCommand_request& e);
  static const std::string toString(const AddCommand_request& e);

  static bool fromJSON(const Json::Value& s,AddCommand_request& e);
  static Json::Value toJSON(const AddCommand_request& e);
};
#endif
