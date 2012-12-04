#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/GetCapabilitiesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 15:06:30 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


GetCapabilitiesResponse& GetCapabilitiesResponse::operator =(const GetCapabilitiesResponse& c)
{
  displayCapabilities=c.displayCapabilities;
  hmiZoneCapabilities=c.hmiZoneCapabilities;
  if(softButtonCapabilities)  delete softButtonCapabilities;
  softButtonCapabilities= c.softButtonCapabilities ? new std::vector<NsAppLinkRPC::SoftButtonCapabilities>(c.softButtonCapabilities[0]) : 0;
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

const std::vector< NsAppLinkRPC::SoftButtonCapabilities>* GetCapabilitiesResponse::get_softButtonCapabilities(void)
{
  return softButtonCapabilities;
}

bool GetCapabilitiesResponse::set_softButtonCapabilities(const std::vector< NsAppLinkRPC::SoftButtonCapabilities>& softButtonCapabilities_)
{
  if(softButtonCapabilities)  delete softButtonCapabilities;
  softButtonCapabilities=new std::vector< NsAppLinkRPC::SoftButtonCapabilities>(softButtonCapabilities_);
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
