//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/TTS/GetLanguageResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::TTS
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::TTS;


GetLanguageResponse& GetLanguageResponse::operator =(const GetLanguageResponse& c)
{
  language=c.language;
  return *this;
}


GetLanguageResponse::~GetLanguageResponse(void)
{
}


GetLanguageResponse::GetLanguageResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_TTS__GETLANGUAGERESPONSE)
{
}


GetLanguageResponse::GetLanguageResponse(const GetLanguageResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_TTS__GETLANGUAGERESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


const NsSmartDeviceLinkRPCV2::Language& GetLanguageResponse::get_language(void)
{
  return language;
}

bool GetLanguageResponse::set_language(const NsSmartDeviceLinkRPCV2::Language& language_)
{
  language=language_;
  return true;
}

bool GetLanguageResponse::checkIntegrity(void)
{
  return GetLanguageResponseMarshaller::checkIntegrity(*this);
}
