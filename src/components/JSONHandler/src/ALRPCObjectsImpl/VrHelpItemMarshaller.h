#ifndef VRHELPITEMMARSHALLER_INCLUDE
#define VRHELPITEMMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/VrHelpItem.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/


struct VrHelpItemMarshaller
{
  static bool checkIntegrity(VrHelpItem& e);
  static bool checkIntegrityConst(const VrHelpItem& e);

  static bool fromString(const std::string& s,VrHelpItem& e);
  static const std::string toString(const VrHelpItem& e);

  static bool fromJSON(const Json::Value& s,VrHelpItem& e);
  static Json::Value toJSON(const VrHelpItem& e);
};
#endif
