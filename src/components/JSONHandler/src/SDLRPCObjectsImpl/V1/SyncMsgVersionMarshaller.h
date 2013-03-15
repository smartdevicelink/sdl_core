//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPC_SYNCMSGVERSIONMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPC_SYNCMSGVERSIONMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V1/SyncMsgVersion.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPC
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
