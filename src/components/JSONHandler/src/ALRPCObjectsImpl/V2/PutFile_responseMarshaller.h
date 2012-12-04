#ifndef NSAPPLINKRPCV2_PUTFILE_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_PUTFILE_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/PutFile_response.h"


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

  struct PutFile_responseMarshaller
  {
    static bool checkIntegrity(PutFile_response& e);
    static bool checkIntegrityConst(const PutFile_response& e);
  
    static bool fromString(const std::string& s,PutFile_response& e);
    static const std::string toString(const PutFile_response& e);
  
    static bool fromJSON(const Json::Value& s,PutFile_response& e);
    static Json::Value toJSON(const PutFile_response& e);
  };
}

#endif
