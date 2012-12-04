#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Buttons/GetCapabilitiesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::Buttons
  version	1.2
  generated at	Tue Dec  4 15:06:30 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsRPC2Communication::Buttons;


GetCapabilitiesResponse& GetCapabilitiesResponse::operator =(const GetCapabilitiesResponse& c)
{
  capabilities=c.capabilities;
  if(presetBankCapabilities)  delete presetBankCapabilities;
  presetBankCapabilities= c.presetBankCapabilities ? new NsAppLinkRPC::PresetBankCapabilities(c.presetBankCapabilities[0]) : 0;
  return *this;
}


GetCapabilitiesResponse::~GetCapabilitiesResponse(void)
{
  if(presetBankCapabilities)  delete presetBankCapabilities;
}


GetCapabilitiesResponse::GetCapabilitiesResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_BUTTONS__GETCAPABILITIESRESPONSE),
  presetBankCapabilities(0)
{
}


GetCapabilitiesResponse::GetCapabilitiesResponse(const GetCapabilitiesResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_BUTTONS__GETCAPABILITIESRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


const std::vector< NsAppLinkRPC::ButtonCapabilities>& GetCapabilitiesResponse::get_capabilities(void)
{
  return capabilities;
}

bool GetCapabilitiesResponse::set_capabilities(const std::vector< NsAppLinkRPC::ButtonCapabilities>& capabilities_)
{
  capabilities=capabilities_;
  return true;
}

const NsAppLinkRPC::PresetBankCapabilities* GetCapabilitiesResponse::get_presetBankCapabilities(void)
{
  return presetBankCapabilities;
}

bool GetCapabilitiesResponse::set_presetBankCapabilities(const NsAppLinkRPC::PresetBankCapabilities& presetBankCapabilities_)
{
  if(presetBankCapabilities)  delete presetBankCapabilities;
  presetBankCapabilities=new NsAppLinkRPC::PresetBankCapabilities(presetBankCapabilities_);
  return true;
}

void GetCapabilitiesResponse::reset_presetBankCapabilities(void)
{
  if(presetBankCapabilities)  delete presetBankCapabilities;
  presetBankCapabilities=0;
}

bool GetCapabilitiesResponse::checkIntegrity(void)
{
  return GetCapabilitiesResponseMarshaller::checkIntegrity(*this);
}
