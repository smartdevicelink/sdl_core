#ifndef CREATEINTERACTIONCHOICESET_REQUESTMARSHALLER_INCLUDE
#define CREATEINTERACTIONCHOICESET_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "../../include/JSONHandler/ALRPCObjects/CreateInteractionChoiceSet_request.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Oct 30 08:29:32 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/


struct CreateInteractionChoiceSet_requestMarshaller
{
  static bool checkIntegrity(CreateInteractionChoiceSet_request& e);
  static bool checkIntegrityConst(const CreateInteractionChoiceSet_request& e);

  static bool fromString(const std::string& s,CreateInteractionChoiceSet_request& e);
  static const std::string toString(const CreateInteractionChoiceSet_request& e);

  static bool fromJSON(const Json::Value& s,CreateInteractionChoiceSet_request& e);
  static Json::Value toJSON(const CreateInteractionChoiceSet_request& e);
};
#endif
