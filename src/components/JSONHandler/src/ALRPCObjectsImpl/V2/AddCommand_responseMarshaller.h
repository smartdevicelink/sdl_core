#ifndef NSAPPLINKRPCV2_ADDCOMMAND_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_ADDCOMMAND_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/AddCommand_response.h"


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

  struct AddCommand_responseMarshaller
  {
    static bool checkIntegrity(AddCommand_response& e);
    static bool checkIntegrityConst(const AddCommand_response& e);
  
    static bool fromString(const std::string& s,AddCommand_response& e);
    static const std::string toString(const AddCommand_response& e);
  
    static bool fromJSON(const Json::Value& s,AddCommand_response& e);
    static Json::Value toJSON(const AddCommand_response& e);
  };
}

#endif
