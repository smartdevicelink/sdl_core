//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPC_ENCODEDSYNCPDATA_REQUESTMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPC_ENCODEDSYNCPDATA_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V1/EncodedSyncPData_request.h"


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

  struct EncodedSyncPData_requestMarshaller
  {
    static bool checkIntegrity(EncodedSyncPData_request& e);
    static bool checkIntegrityConst(const EncodedSyncPData_request& e);
  
    static bool fromString(const std::string& s,EncodedSyncPData_request& e);
    static const std::string toString(const EncodedSyncPData_request& e);
  
    static bool fromJSON(const Json::Value& s,EncodedSyncPData_request& e);
    static Json::Value toJSON(const EncodedSyncPData_request& e);
  };
}

#endif
