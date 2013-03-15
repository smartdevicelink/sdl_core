//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/TTS/GetCapabilitiesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::TTS
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::TTS;


GetCapabilitiesResponse& GetCapabilitiesResponse::operator =(const GetCapabilitiesResponse& c)
{
  capabilities=c.capabilities;
  return *this;
}


GetCapabilitiesResponse::~GetCapabilitiesResponse(void)
{
}


GetCapabilitiesResponse::GetCapabilitiesResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_TTS__GETCAPABILITIESRESPONSE)
{
}


GetCapabilitiesResponse::GetCapabilitiesResponse(const GetCapabilitiesResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_TTS__GETCAPABILITIESRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


const std::vector< NsSmartDeviceLinkRPCV2::SpeechCapabilities>& GetCapabilitiesResponse::get_capabilities(void)
{
  return capabilities;
}

bool GetCapabilitiesResponse::set_capabilities(const std::vector< NsSmartDeviceLinkRPCV2::SpeechCapabilities>& capabilities_)
{
  capabilities=capabilities_;
  return true;
}

bool GetCapabilitiesResponse::checkIntegrity(void)
{
  return GetCapabilitiesResponseMarshaller::checkIntegrity(*this);
}
