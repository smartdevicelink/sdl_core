#ifndef DELETEINTERACTIONCHOICESET_RESPONSEMARSHALLER_INCLUDE
#define DELETEINTERACTIONCHOICESET_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/DeleteInteractionChoiceSet_response.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
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
