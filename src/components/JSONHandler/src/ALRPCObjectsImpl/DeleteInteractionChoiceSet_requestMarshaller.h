#ifndef DELETEINTERACTIONCHOICESET_REQUESTMARSHALLER_INCLUDE
#define DELETEINTERACTIONCHOICESET_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "../../include/JSONHandler/ALRPCObjects/DeleteInteractionChoiceSet_request.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
  author	robok0der
*/


struct DeleteInteractionChoiceSet_requestMarshaller
{
  static bool checkIntegrity(DeleteInteractionChoiceSet_request& e);
  static bool checkIntegrityConst(const DeleteInteractionChoiceSet_request& e);

  static bool fromString(const std::string& s,DeleteInteractionChoiceSet_request& e);
  static const std::string toString(const DeleteInteractionChoiceSet_request& e);

  static bool fromJSON(const Json::Value& s,DeleteInteractionChoiceSet_request& e);
  static Json::Value toJSON(const DeleteInteractionChoiceSet_request& e);
};
#endif
