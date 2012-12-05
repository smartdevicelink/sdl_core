#ifndef NSAPPLINKRPCV2_SOFTBUTTONMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_SOFTBUTTONMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/SoftButton.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  struct SoftButtonMarshaller
  {
    static bool checkIntegrity(SoftButton& e);
    static bool checkIntegrityConst(const SoftButton& e);
  
    static bool fromString(const std::string& s,SoftButton& e);
    static const std::string toString(const SoftButton& e);
  
    static bool fromJSON(const Json::Value& s,SoftButton& e);
    static Json::Value toJSON(const SoftButton& e);
  };
}

#endif
