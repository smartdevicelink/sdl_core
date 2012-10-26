#ifndef CREATEINTERACTIONCHOICESET_RESPONSEMARSHALLER_INCLUDE
#define CREATEINTERACTIONCHOICESET_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "../../include/JSONHandler/ALRPCObjects/CreateInteractionChoiceSet_response.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/


struct CreateInteractionChoiceSet_responseMarshaller
{
  static bool checkIntegrity(CreateInteractionChoiceSet_response& e);
  static bool checkIntegrityConst(const CreateInteractionChoiceSet_response& e);

  static bool fromString(const std::string& s,CreateInteractionChoiceSet_response& e);
  static const std::string toString(const CreateInteractionChoiceSet_response& e);

  static bool fromJSON(const Json::Value& s,CreateInteractionChoiceSet_response& e);
  static Json::Value toJSON(const CreateInteractionChoiceSet_response& e);
};
#endif
