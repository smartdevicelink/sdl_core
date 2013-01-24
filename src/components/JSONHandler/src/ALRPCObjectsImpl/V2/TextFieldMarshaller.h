#ifndef NSAPPLINKRPCV2_TEXTFIELDMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_TEXTFIELDMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/TextField.h"


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

  struct TextFieldMarshaller
  {
    static bool checkIntegrity(TextField& e);
    static bool checkIntegrityConst(const TextField& e);
  
    static bool fromString(const std::string& s,TextField& e);
    static const std::string toString(const TextField& e);
  
    static bool fromJSON(const Json::Value& s,TextField& e);
    static Json::Value toJSON(const TextField& e);
  };
}

#endif
