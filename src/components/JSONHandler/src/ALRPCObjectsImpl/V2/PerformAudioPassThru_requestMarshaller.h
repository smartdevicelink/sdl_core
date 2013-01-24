#ifndef NSAPPLINKRPCV2_PERFORMAUDIOPASSTHRU_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_PERFORMAUDIOPASSTHRU_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/PerformAudioPassThru_request.h"


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

  struct PerformAudioPassThru_requestMarshaller
  {
    static bool checkIntegrity(PerformAudioPassThru_request& e);
    static bool checkIntegrityConst(const PerformAudioPassThru_request& e);
  
    static bool fromString(const std::string& s,PerformAudioPassThru_request& e);
    static const std::string toString(const PerformAudioPassThru_request& e);
  
    static bool fromJSON(const Json::Value& s,PerformAudioPassThru_request& e);
    static Json::Value toJSON(const PerformAudioPassThru_request& e);
  };
}

#endif
