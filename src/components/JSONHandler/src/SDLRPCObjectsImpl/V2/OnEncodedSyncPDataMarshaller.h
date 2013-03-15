//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_ONENCODEDSYNCPDATAMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_ONENCODEDSYNCPDATAMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/OnEncodedSyncPData.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPCV2
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
