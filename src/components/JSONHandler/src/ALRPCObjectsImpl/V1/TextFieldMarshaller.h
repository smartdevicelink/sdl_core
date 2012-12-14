#ifndef NSAPPLINKRPC_TEXTFIELDMARSHALLER_INCLUDE
#define NSAPPLINKRPC_TEXTFIELDMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V1/TextField.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Dec 13 13:37:09 2012
  source stamp	Thu Dec 13 13:33:23 2012
  author	robok0der
*/

namespace NsAppLinkRPC
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
