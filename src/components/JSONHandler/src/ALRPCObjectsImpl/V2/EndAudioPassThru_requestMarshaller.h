#ifndef NSAPPLINKRPCV2_ENDAUDIOPASSTHRU_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_ENDAUDIOPASSTHRU_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/EndAudioPassThru_request.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  struct EndAudioPassThru_requestMarshaller
  {
    static bool checkIntegrity(EndAudioPassThru_request& e);
    static bool checkIntegrityConst(const EndAudioPassThru_request& e);
  
    static bool fromString(const std::string& s,EndAudioPassThru_request& e);
    static const std::string toString(const EndAudioPassThru_request& e);
  
    static bool fromJSON(const Json::Value& s,EndAudioPassThru_request& e);
    static Json::Value toJSON(const EndAudioPassThru_request& e);
  };
}

#endif
