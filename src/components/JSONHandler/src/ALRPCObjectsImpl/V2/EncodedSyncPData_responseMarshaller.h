#ifndef NSAPPLINKRPCV2_ENCODEDSYNCPDATA_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_ENCODEDSYNCPDATA_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/EncodedSyncPData_response.h"


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
