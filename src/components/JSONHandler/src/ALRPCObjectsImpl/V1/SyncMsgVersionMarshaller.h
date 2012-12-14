#ifndef NSAPPLINKRPC_SYNCMSGVERSIONMARSHALLER_INCLUDE
#define NSAPPLINKRPC_SYNCMSGVERSIONMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V1/SyncMsgVersion.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Dec 13 13:37:09 2012
  source stamp	Thu Dec 13 13:33:23 2012
  author	robok0der
*/

namespace NsAppLinkRPC
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
