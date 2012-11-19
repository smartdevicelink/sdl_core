#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/GetCapabilitiesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Mon Nov 19 12:18:27 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


GetCapabilitiesResponse& GetCapabilitiesResponse::operator =(const GetCapabilitiesResponse& c)
{
  displayCapabilities=c.displayCapabilities;
  hmiZoneCapabilities=c.hmiZoneCapabilities;
  return *this;
}


GetCapabilitiesResponse::~GetCapabilitiesResponse(void)
{
}


GetCapabilitiesResponse::GetCapabilitiesResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__GETCAPABILITIESRESPONSE)
{
}


GetCapabilitiesResponse::GetCapabilitiesResponse(const GetCapabilitiesResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__GETCAPABILITIESRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


const NsAppLinkRPC::DisplayCapabilities& GetCapabilitiesResponse::get_displayCapabilities(void)
{
  return displayCapabilities;
}

bool GetCapabilitiesResponse::set_displayCapabilities(const NsAppLinkRPC::DisplayCapabilities& displayCapabilities_)
{
  displayCapabilities=displayCapabilities_;
  return true;
}

const std::vector< NsAppLinkRPC::HmiZoneCapabilities>& GetCapabilitiesResponse::get_hmiZoneCapabilities(void)
{
  return hmiZoneCapabilities;
}

bool GetCapabilitiesResponse::set_hmiZoneCapabilities(const std::vector< NsAppLinkRPC::HmiZoneCapabilities>& hmiZoneCapabilities_)
{
  hmiZoneCapabilities=hmiZoneCapabilities_;
  return true;
}

bool GetCapabilitiesResponse::checkIntegrity(void)
{
  return GetCapabilitiesResponseMarshaller::checkIntegrity(*this);
}
