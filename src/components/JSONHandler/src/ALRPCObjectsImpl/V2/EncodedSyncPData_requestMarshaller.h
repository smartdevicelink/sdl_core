#ifndef NSAPPLINKRPCV2_ENCODEDSYNCPDATA_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_ENCODEDSYNCPDATA_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/EncodedSyncPData_request.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
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
