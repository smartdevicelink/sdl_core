//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_SYNCMSGVERSIONMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_SYNCMSGVERSIONMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/SyncMsgVersion.h"


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

  struct SyncMsgVersionMarshaller
  {
    static bool checkIntegrity(SyncMsgVersion& e);
    static bool checkIntegrityConst(const SyncMsgVersion& e);
  
    static bool fromString(const std::string& s,SyncMsgVersion& e);
    static const std::string toString(const SyncMsgVersion& e);
  
    static bool fromJSON(const Json::Value& s,SyncMsgVersion& e);
    static Json::Value toJSON(const SyncMsgVersion& e);
  };
}

#endif
