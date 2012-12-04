#ifndef NSAPPLINKRPCV2_SINGLETIRESTATUSMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_SINGLETIRESTATUSMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/SingleTireStatus.h"


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

  struct SingleTireStatusMarshaller
  {
    static bool checkIntegrity(SingleTireStatus& e);
    static bool checkIntegrityConst(const SingleTireStatus& e);
  
    static bool fromString(const std::string& s,SingleTireStatus& e);
    static const std::string toString(const SingleTireStatus& e);
  
    static bool fromJSON(const Json::Value& s,SingleTireStatus& e);
    static Json::Value toJSON(const SingleTireStatus& e);
  };
}

#endif
