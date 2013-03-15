//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_ENDAUDIOPASSTHRU_REQUESTMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_ENDAUDIOPASSTHRU_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/EndAudioPassThru_request.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPCV2
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
