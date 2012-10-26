#ifndef DELETEINTERACTIONCHOICESET_RESPONSEMARSHALLER_INCLUDE
#define DELETEINTERACTIONCHOICESET_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "../../include/JSONHandler/ALRPCObjects/DeleteInteractionChoiceSet_response.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/


struct DeleteInteractionChoiceSet_responseMarshaller
{
  static bool checkIntegrity(DeleteInteractionChoiceSet_response& e);
  static bool checkIntegrityConst(const DeleteInteractionChoiceSet_response& e);

  static bool fromString(const std::string& s,DeleteInteractionChoiceSet_response& e);
  static const std::string toString(const DeleteInteractionChoiceSet_response& e);

  static bool fromJSON(const Json::Value& s,DeleteInteractionChoiceSet_response& e);
  static Json::Value toJSON(const DeleteInteractionChoiceSet_response& e);
};
#endif
