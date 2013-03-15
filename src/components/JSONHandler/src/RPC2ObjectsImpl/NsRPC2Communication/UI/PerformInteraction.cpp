//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/PerformInteraction.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::UI;


PerformInteraction& PerformInteraction::operator =(const PerformInteraction& c)
{
  initialText=c.initialText;
  initialPrompt=c.initialPrompt;
  interactionMode=c.interactionMode;
  interactionChoiceSetIDList=c.interactionChoiceSetIDList;
  if(helpPrompt)  delete helpPrompt;
  helpPrompt= c.helpPrompt ? new std::vector<NsSmartDeviceLinkRPCV2::TTSChunk>(c.helpPrompt[0]) : 0;
  if(timeoutPrompt)  delete timeoutPrompt;
  timeoutPrompt= c.timeoutPrompt ? new std::vector<NsSmartDeviceLinkRPCV2::TTSChunk>(c.timeoutPrompt[0]) : 0;
  if(timeout)  delete timeout;
  timeout= c.timeout ? new unsigned int(c.timeout[0]) : 0;
  if(vrHelp)  delete vrHelp;
  vrHelp= c.vrHelp ? new std::vector<NsSmartDeviceLinkRPCV2::VrHelpItem>(c.vrHelp[0]) : 0;
  appId=c.appId;
  return *this;
}


PerformInteraction::~PerformInteraction(void)
{
  if(helpPrompt)  delete helpPrompt;
  if(timeoutPrompt)  delete timeoutPrompt;
  if(timeout)  delete timeout;
  if(vrHelp)  delete vrHelp;
}


PerformInteraction::PerformInteraction(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__PERFORMINTERACTION),
  helpPrompt(0),
  timeoutPrompt(0),
  timeout(0),
  vrHelp(0)
{
}


PerformInteraction::PerformInteraction(const PerformInteraction& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__PERFORMINTERACTION,c.getId())
{
  *this=c;
}


const std::string& PerformInteraction::get_initialText(void)
{
  return initialText;
}

bool PerformInteraction::set_initialText(const std::string& initialText_)
{
  initialText=initialText_;
  return true;
}

const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>& PerformInteraction::get_initialPrompt(void)
{
  return initialPrompt;
}

bool PerformInteraction::set_initialPrompt(const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>& initialPrompt_)
{
  initialPrompt=initialPrompt_;
  return true;
}

const NsSmartDeviceLinkRPCV2::InteractionMode& PerformInteraction::get_interactionMode(void)
{
  return interactionMode;
}

bool PerformInteraction::set_interactionMode(const NsSmartDeviceLinkRPCV2::InteractionMode& interactionMode_)
{
  interactionMode=interactionMode_;
  return true;
}

const std::vector< unsigned int>& PerformInteraction::get_interactionChoiceSetIDList(void)
{
  return interactionChoiceSetIDList;
}

bool PerformInteraction::set_interactionChoiceSetIDList(const std::vector< unsigned int>& interactionChoiceSetIDList_)
{
  interactionChoiceSetIDList=interactionChoiceSetIDList_;
  return true;
}

const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>* PerformInteraction::get_helpPrompt(void)
{
  return helpPrompt;
}

bool PerformInteraction::set_helpPrompt(const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>& helpPrompt_)
{
  if(helpPrompt)  delete helpPrompt;
  helpPrompt=new std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>(helpPrompt_);
  return true;
}

void PerformInteraction::reset_helpPrompt(void)
{
  if(helpPrompt)  delete helpPrompt;
  helpPrompt=0;
}

const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>* PerformInteraction::get_timeoutPrompt(void)
{
  return timeoutPrompt;
}

bool PerformInteraction::set_timeoutPrompt(const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>& timeoutPrompt_)
{
  if(timeoutPrompt)  delete timeoutPrompt;
  timeoutPrompt=new std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>(timeoutPrompt_);
  return true;
}

void PerformInteraction::reset_timeoutPrompt(void)
{
  if(timeoutPrompt)  delete timeoutPrompt;
  timeoutPrompt=0;
}

const unsigned int* PerformInteraction::get_timeout(void)
{
  return timeout;
}

bool PerformInteraction::set_timeout(const unsigned int& timeout_)
{
  if(timeout)  delete timeout;
  timeout=new unsigned int(timeout_);
  return true;
}

void PerformInteraction::reset_timeout(void)
{
  if(timeout)  delete timeout;
  timeout=0;
}

const std::vector< NsSmartDeviceLinkRPCV2::VrHelpItem>* PerformInteraction::get_vrHelp(void)
{
  return vrHelp;
}

bool PerformInteraction::set_vrHelp(const std::vector< NsSmartDeviceLinkRPCV2::VrHelpItem>& vrHelp_)
{
  if(vrHelp)  delete vrHelp;
  vrHelp=new std::vector< NsSmartDeviceLinkRPCV2::VrHelpItem>(vrHelp_);
  return true;
}

void PerformInteraction::reset_vrHelp(void)
{
  if(vrHelp)  delete vrHelp;
  vrHelp=0;
}

int PerformInteraction::get_appId(void)
{
  return appId;
}

bool PerformInteraction::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool PerformInteraction::checkIntegrity(void)
{
  return PerformInteractionMarshaller::checkIntegrity(*this);
}
