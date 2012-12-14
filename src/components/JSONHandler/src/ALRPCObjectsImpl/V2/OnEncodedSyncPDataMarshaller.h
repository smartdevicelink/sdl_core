#ifndef NSAPPLINKRPCV2_ONENCODEDSYNCPDATAMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_ONENCODEDSYNCPDATAMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/OnEncodedSyncPData.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  struct OnEncodedSyncPDataMarshaller
  {
    static bool checkIntegrity(OnEncodedSyncPData& e);
    static bool checkIntegrityConst(const OnEncodedSyncPData& e);
  
    static bool fromString(const std::string& s,OnEncodedSyncPData& e);
    static const std::string toString(const OnEncodedSyncPData& e);
  
    static bool fromJSON(const Json::Value& s,OnEncodedSyncPData& e);
    static Json::Value toJSON(const OnEncodedSyncPData& e);
  };
}

#endif
