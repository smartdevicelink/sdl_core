#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/GetCapabilitiesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


GetCapabilitiesResponse& GetCapabilitiesResponse::operator =(const GetCapabilitiesResponse& c)
{
  displayCapabilities=c.displayCapabilities;
  hmiZoneCapabilities=c.hmiZoneCapabilities;
  if(softButtonCapabilities)  delete softButtonCapabilities;
  softButtonCapabilities= c.softButtonCapabilities ? new std::vector<NsAppLinkRPCV2::SoftButtonCapabilities>(c.softButtonCapabilities[0]) : 0;
  return *this;
}


GetCapabilitiesResponse::~GetCapabilitiesResponse(void)
{
  if(softButtonCapabilities)  delete softButtonCapabilities;
}


GetCapabilitiesResponse::GetCapabilitiesResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__GETCAPABILITIESRESPONSE),
  softButtonCapabilities(0)
{
}


GetCapabilitiesResponse::GetCapabilitiesResponse(const GetCapabilitiesResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__GETCAPABILITIESRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


const NsAppLinkRPCV2::DisplayCapabilities& GetCapabilitiesResponse::get_displayCapabilities(void)
{
  return displayCapabilities;
}

bool GetCapabilitiesResponse::set_displayCapabilities(const NsAppLinkRPCV2::DisplayCapabilities& displayCapabilities_)
{
  displayCapabilities=displayCapabilities_;
  return true;
}

const std::vector< NsAppLinkRPCV2::HmiZoneCapabilities>& GetCapabilitiesResponse::get_hmiZoneCapabilities(void)
{
  return hmiZoneCapabilities;
}

bool GetCapabilitiesResponse::set_hmiZoneCapabilities(const std::vector< NsAppLinkRPCV2::HmiZoneCapabilities>& hmiZoneCapabilities_)
{
  hmiZoneCapabilities=hmiZoneCapabilities_;
  return true;
}

const std::vector< NsAppLinkRPCV2::SoftButtonCapabilities>* GetCapabilitiesResponse::get_softButtonCapabilities(void)
{
  return softButtonCapabilities;
}

bool GetCapabilitiesResponse::set_softButtonCapabilities(const std::vector< NsAppLinkRPCV2::SoftButtonCapabilities>& softButtonCapabilities_)
{
  if(softButtonCapabilities)  delete softButtonCapabilities;
  softButtonCapabilities=new std::vector< NsAppLinkRPCV2::SoftButtonCapabilities>(softButtonCapabilities_);
  return true;
}

void GetCapabilitiesResponse::reset_softButtonCapabilities(void)
{
  if(softButtonCapabilities)  delete softButtonCapabilities;
  softButtonCapabilities=0;
}

bool GetCapabilitiesResponse::checkIntegrity(void)
{
  return GetCapabilitiesResponseMarshaller::checkIntegrity(*this);
}
