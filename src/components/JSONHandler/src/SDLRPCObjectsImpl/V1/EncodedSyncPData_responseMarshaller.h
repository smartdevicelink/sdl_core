#ifndef NSAPPLINKRPC_ENCODEDSYNCPDATA_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_ENCODEDSYNCPDATA_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V1/EncodedSyncPData_response.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	robok0der
*/

namespace NsSmartDeviceLinkRPC
{

  struct EncodedSyncPData_responseMarshaller
  {
    static bool checkIntegrity(EncodedSyncPData_response& e);
    static bool checkIntegrityConst(const EncodedSyncPData_response& e);
  
    static bool fromString(const std::string& s,EncodedSyncPData_response& e);
    static const std::string toString(const EncodedSyncPData_response& e);
  
    static bool fromJSON(const Json::Value& s,EncodedSyncPData_response& e);
    static Json::Value toJSON(const EncodedSyncPData_response& e);
  };
}

#endif
