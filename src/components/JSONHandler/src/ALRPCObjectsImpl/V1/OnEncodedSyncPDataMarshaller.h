#ifndef NSAPPLINKRPCV1_ONENCODEDSYNCPDATAMARSHALLER_INCLUDE
#define NSAPPLINKRPCV1_ONENCODEDSYNCPDATAMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V1/OnEncodedSyncPData.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 14:55:10 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV1
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
