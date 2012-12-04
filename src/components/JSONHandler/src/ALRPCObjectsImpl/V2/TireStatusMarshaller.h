#ifndef NSAPPLINKRPCV2_TIRESTATUSMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_TIRESTATUSMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/TireStatus.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  struct TireStatusMarshaller
  {
    static bool checkIntegrity(TireStatus& e);
    static bool checkIntegrityConst(const TireStatus& e);
  
    static bool fromString(const std::string& s,TireStatus& e);
    static const std::string toString(const TireStatus& e);
  
    static bool fromJSON(const Json::Value& s,TireStatus& e);
    static Json::Value toJSON(const TireStatus& e);
  };
}

#endif
