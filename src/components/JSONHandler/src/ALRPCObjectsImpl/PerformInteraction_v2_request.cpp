#include "../include/JSONHandler/ALRPCObjects/PerformInteraction_v2_request.h"
#include "PerformInteraction_v2_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "InteractionModeMarshaller.h"
#include "TTSChunk_v2Marshaller.h"
#include "VrHelpItemMarshaller.h"

#define PROTOCOL_VERSION	42


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;
PerformInteraction_v2_request& PerformInteraction_v2_request::operator =(const PerformInteraction_v2_request& c)
{
  initialText= c.initialText;
  initialPrompt= c.initialPrompt;
  interactionMode= c.interactionMode;
  interactionChoiceSetIDList= c.interactionChoiceSetIDList;
  helpPrompt= c.helpPrompt ? new std::vector<TTSChunk_v2>(c.helpPrompt[0]) : 0;
  timeoutPrompt= c.timeoutPrompt ? new std::vector<TTSChunk_v2>(c.timeoutPrompt[0]) : 0;
  timeout= c.timeout ? new unsigned int(c.timeout[0]) : 0;
  vrHelp= c.vrHelp ? new std::vector<VrHelpItem>(c.vrHelp[0]) : 0;

  return *this;}


PerformInteraction_v2_request::~PerformInteraction_v2_request(void)
{
  if(helpPrompt)
    delete helpPrompt;
  if(timeoutPrompt)
    delete timeoutPrompt;
  if(timeout)
    delete timeout;
  if(vrHelp)
    delete vrHelp;
}


PerformInteraction_v2_request::PerformInteraction_v2_request(const PerformInteraction_v2_request& c)
{
  *this=c;
}


bool PerformInteraction_v2_request::checkIntegrity(void)
{
  return PerformInteraction_v2_requestMarshaller::checkIntegrity(*this);
}


PerformInteraction_v2_request::PerformInteraction_v2_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_PERFORMINTERACTION_V2_REQUEST),
      helpPrompt(0),
    timeoutPrompt(0),
    timeout(0),
    vrHelp(0)
{
}



bool PerformInteraction_v2_request::set_initialText(const std::string& initialText_)
{
  if(initialText_.length()>500)  return false;
  initialText=initialText_;
  return true;
}

bool PerformInteraction_v2_request::set_initialPrompt(const std::vector<TTSChunk_v2>& initialPrompt_)
{
  unsigned int i=initialPrompt_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!TTSChunk_v2Marshaller::checkIntegrityConst(initialPrompt_[i]))   return false;
  }
  initialPrompt=initialPrompt_;
  return true;
}

bool PerformInteraction_v2_request::set_interactionMode(const InteractionMode& interactionMode_)
{
  if(!InteractionModeMarshaller::checkIntegrityConst(interactionMode_))   return false;
  interactionMode=interactionMode_;
  return true;
}

bool PerformInteraction_v2_request::set_interactionChoiceSetIDList(const std::vector<unsigned int>& interactionChoiceSetIDList_)
{
  unsigned int i=interactionChoiceSetIDList_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(interactionChoiceSetIDList_[i]>2000000000)  return false;
  }
  interactionChoiceSetIDList=interactionChoiceSetIDList_;
  return true;
}

bool PerformInteraction_v2_request::set_helpPrompt(const std::vector<TTSChunk_v2>& helpPrompt_)
{
  unsigned int i=helpPrompt_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!TTSChunk_v2Marshaller::checkIntegrityConst(helpPrompt_[i]))   return false;
  }
  delete helpPrompt;
  helpPrompt=0;

  helpPrompt=new std::vector<TTSChunk_v2>(helpPrompt_);
  return true;
}

void PerformInteraction_v2_request::reset_helpPrompt(void)
{
  if(helpPrompt)
    delete helpPrompt;
  helpPrompt=0;
}

bool PerformInteraction_v2_request::set_timeoutPrompt(const std::vector<TTSChunk_v2>& timeoutPrompt_)
{
  unsigned int i=timeoutPrompt_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!TTSChunk_v2Marshaller::checkIntegrityConst(timeoutPrompt_[i]))   return false;
  }
  delete timeoutPrompt;
  timeoutPrompt=0;

  timeoutPrompt=new std::vector<TTSChunk_v2>(timeoutPrompt_);
  return true;
}

void PerformInteraction_v2_request::reset_timeoutPrompt(void)
{
  if(timeoutPrompt)
    delete timeoutPrompt;
  timeoutPrompt=0;
}

bool PerformInteraction_v2_request::set_timeout(unsigned int timeout_)
{
  if(timeout_>100000)  return false;
  if(timeout_<5000)  return false;
  delete timeout;
  timeout=0;

  timeout=new unsigned int(timeout_);
  return true;
}

void PerformInteraction_v2_request::reset_timeout(void)
{
  if(timeout)
    delete timeout;
  timeout=0;
}

bool PerformInteraction_v2_request::set_vrHelp(const std::vector<VrHelpItem>& vrHelp_)
{
  unsigned int i=vrHelp_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!VrHelpItemMarshaller::checkIntegrityConst(vrHelp_[i]))   return false;
  }
  delete vrHelp;
  vrHelp=0;

  vrHelp=new std::vector<VrHelpItem>(vrHelp_);
  return true;
}

void PerformInteraction_v2_request::reset_vrHelp(void)
{
  if(vrHelp)
    delete vrHelp;
  vrHelp=0;
}




const std::string& PerformInteraction_v2_request::get_initialText(void) const 
{
  return initialText;
}

const std::vector<TTSChunk_v2>& PerformInteraction_v2_request::get_initialPrompt(void) const 
{
  return initialPrompt;
}

const InteractionMode& PerformInteraction_v2_request::get_interactionMode(void) const 
{
  return interactionMode;
}

const std::vector<unsigned int>& PerformInteraction_v2_request::get_interactionChoiceSetIDList(void) const 
{
  return interactionChoiceSetIDList;
}

const std::vector<TTSChunk_v2>* PerformInteraction_v2_request::get_helpPrompt(void) const 
{
  return helpPrompt;
}

const std::vector<TTSChunk_v2>* PerformInteraction_v2_request::get_timeoutPrompt(void) const 
{
  return timeoutPrompt;
}

const unsigned int* PerformInteraction_v2_request::get_timeout(void) const 
{
  return timeout;
}

const std::vector<VrHelpItem>* PerformInteraction_v2_request::get_vrHelp(void) const 
{
  return vrHelp;
}

