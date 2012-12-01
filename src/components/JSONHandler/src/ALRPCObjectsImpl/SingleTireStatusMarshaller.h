#ifndef NSAPPLINKRPC_SINGLETIRESTATUSMARSHALLER_INCLUDE
#define NSAPPLINKRPC_SINGLETIRESTATUSMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/SingleTireStatus.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
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
