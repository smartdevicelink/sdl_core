#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/SetGlobalProperties.h"
#include "../../../../../Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Mon Nov 19 10:37:39 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


SetGlobalProperties& SetGlobalProperties::operator =(const SetGlobalProperties& c)
{
  if(helpPrompt)  delete helpPrompt;
  helpPrompt= c.helpPrompt ? new std::vector<NsAppLinkRPC::TTSChunk>(c.helpPrompt[0]) : 0;
  if(timeoutPrompt)  delete timeoutPrompt;
  timeoutPrompt= c.timeoutPrompt ? new std::vector<NsAppLinkRPC::TTSChunk>(c.timeoutPrompt[0]) : 0;
  return *this;
}


SetGlobalProperties::~SetGlobalProperties(void)
{
  if(helpPrompt)  delete helpPrompt;
  if(timeoutPrompt)  delete timeoutPrompt;
}


SetGlobalProperties::SetGlobalProperties(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SETGLOBALPROPERTIES),
  helpPrompt(0),
  timeoutPrompt(0)
{
}


SetGlobalProperties::SetGlobalProperties(const SetGlobalProperties& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SETGLOBALPROPERTIES,c.getId())
{
  *this=c;
}


const std::vector< NsAppLinkRPC::TTSChunk>* SetGlobalProperties::get_helpPrompt(void)
{
  return helpPrompt;
}

bool SetGlobalProperties::set_helpPrompt(const std::vector< NsAppLinkRPC::TTSChunk>& helpPrompt_)
{
  if(helpPrompt)  delete helpPrompt;
  helpPrompt=new std::vector< NsAppLinkRPC::TTSChunk>(helpPrompt_);
  return true;
}

void SetGlobalProperties::reset_helpPrompt(void)
{
  if(helpPrompt)  delete helpPrompt;
  helpPrompt=0;
}

const std::vector< NsAppLinkRPC::TTSChunk>* SetGlobalProperties::get_timeoutPrompt(void)
{
  return timeoutPrompt;
}

bool SetGlobalProperties::set_timeoutPrompt(const std::vector< NsAppLinkRPC::TTSChunk>& timeoutPrompt_)
{
  if(timeoutPrompt)  delete timeoutPrompt;
  timeoutPrompt=new std::vector< NsAppLinkRPC::TTSChunk>(timeoutPrompt_);
  return true;
}

void SetGlobalProperties::reset_timeoutPrompt(void)
{
  if(timeoutPrompt)  delete timeoutPrompt;
  timeoutPrompt=0;
}

bool SetGlobalProperties::checkIntegrity(void)
{
  return SetGlobalPropertiesMarshaller::checkIntegrity(*this);
}
