#ifndef NSAPPLINKRPC_ONENCODEDSYNCPDATAMARSHALLER_INCLUDE
#define NSAPPLINKRPC_ONENCODEDSYNCPDATAMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V1/OnEncodedSyncPData.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	robok0der
*/

namespace NsAppLinkRPC
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
