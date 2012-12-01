#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/SetGlobalProperties.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Nov 29 14:32:09 2012
  source stamp	Thu Nov 29 14:32:05 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


SetGlobalProperties& SetGlobalProperties::operator =(const SetGlobalProperties& c)
{
  if(helpPrompt)  delete helpPrompt;
  helpPrompt= c.helpPrompt ? new std::vector<NsAppLinkRPC::TTSChunk>(c.helpPrompt[0]) : 0;
  if(timeoutPrompt)  delete timeoutPrompt;
  timeoutPrompt= c.timeoutPrompt ? new std::vector<NsAppLinkRPC::TTSChunk>(c.timeoutPrompt[0]) : 0;
  if(vrHelpTitle)  delete vrHelpTitle;
  vrHelpTitle= c.vrHelpTitle ? new std::string(c.vrHelpTitle[0]) : 0;
  if(vrHelp)  delete vrHelp;
  vrHelp= c.vrHelp ? new std::vector<NsAppLinkRPC::VrHelpItem>(c.vrHelp[0]) : 0;
  appId=c.appId;
  return *this;
}


SetGlobalProperties::~SetGlobalProperties(void)
{
  if(helpPrompt)  delete helpPrompt;
  if(timeoutPrompt)  delete timeoutPrompt;
  if(vrHelpTitle)  delete vrHelpTitle;
  if(vrHelp)  delete vrHelp;
}


SetGlobalProperties::SetGlobalProperties(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SETGLOBALPROPERTIES),
  helpPrompt(0),
  timeoutPrompt(0),
  vrHelpTitle(0),
  vrHelp(0)
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

const std::string* SetGlobalProperties::get_vrHelpTitle(void)
{
  return vrHelpTitle;
}

bool SetGlobalProperties::set_vrHelpTitle(const std::string& vrHelpTitle_)
{
  if(vrHelpTitle)  delete vrHelpTitle;
  vrHelpTitle=new std::string(vrHelpTitle_);
  return true;
}

void SetGlobalProperties::reset_vrHelpTitle(void)
{
  if(vrHelpTitle)  delete vrHelpTitle;
  vrHelpTitle=0;
}

const std::vector< NsAppLinkRPC::VrHelpItem>* SetGlobalProperties::get_vrHelp(void)
{
  return vrHelp;
}

bool SetGlobalProperties::set_vrHelp(const std::vector< NsAppLinkRPC::VrHelpItem>& vrHelp_)
{
  if(vrHelp)  delete vrHelp;
  vrHelp=new std::vector< NsAppLinkRPC::VrHelpItem>(vrHelp_);
  return true;
}

void SetGlobalProperties::reset_vrHelp(void)
{
  if(vrHelp)  delete vrHelp;
  vrHelp=0;
}

int SetGlobalProperties::get_appId(void)
{
  return appId;
}

bool SetGlobalProperties::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool SetGlobalProperties::checkIntegrity(void)
{
  return SetGlobalPropertiesMarshaller::checkIntegrity(*this);
}
