#ifndef NSAPPLINKRPCV2_UPDATETURNLIST_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_UPDATETURNLIST_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/UpdateTurnList_request.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  struct UpdateTurnList_requestMarshaller
  {
    static bool checkIntegrity(UpdateTurnList_request& e);
    static bool checkIntegrityConst(const UpdateTurnList_request& e);
  
    static bool fromString(const std::string& s,UpdateTurnList_request& e);
    static const std::string toString(const UpdateTurnList_request& e);
  
    static bool fromJSON(const Json::Value& s,UpdateTurnList_request& e);
    static Json::Value toJSON(const UpdateTurnList_request& e);
  };
}

#endif
