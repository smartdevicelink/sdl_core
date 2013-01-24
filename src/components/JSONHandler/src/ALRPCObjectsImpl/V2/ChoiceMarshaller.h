#ifndef NSAPPLINKRPCV2_CHOICEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_CHOICEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/Choice.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  struct ChoiceMarshaller
  {
    static bool checkIntegrity(Choice& e);
    static bool checkIntegrityConst(const Choice& e);
  
    static bool fromString(const std::string& s,Choice& e);
    static const std::string toString(const Choice& e);
  
    static bool fromJSON(const Json::Value& s,Choice& e);
    static Json::Value toJSON(const Choice& e);
  };
}

#endif
