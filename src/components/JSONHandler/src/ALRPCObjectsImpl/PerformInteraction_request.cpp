#include "../../include/JSONHandler/ALRPCObjects/PerformInteraction_request.h"
#include "PerformInteraction_requestMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "InteractionModeMarshaller.h"
#include "TTSChunkMarshaller.h"
#include "VrHelpItemMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/

PerformInteraction_request& PerformInteraction_request::operator =(const PerformInteraction_request& c)
{
  initialText= c.initialText;
  initialPrompt= c.initialPrompt;
  interactionMode= c.interactionMode;
  interactionChoiceSetIDList= c.interactionChoiceSetIDList;
  helpPrompt= c.helpPrompt ? new std::vector<TTSChunk>(c.helpPrompt[0]) : 0;
  timeoutPrompt= c.timeoutPrompt ? new std::vector<TTSChunk>(c.timeoutPrompt[0]) : 0;
  timeout= c.timeout ? new unsigned int(c.timeout[0]) : 0;
  vrHelpTitle= c.vrHelpTitle ? new std::string(c.vrHelpTitle[0]) : 0;
  vrHelp= c.vrHelp ? new std::vector<VrHelpItem>(c.vrHelp[0]) : 0;

  return *this;}


PerformInteraction_request::~PerformInteraction_request(void)
{
  if(helpPrompt)
    delete helpPrompt;
  if(timeoutPrompt)
    delete timeoutPrompt;
  if(timeout)
    delete timeout;
  if(vrHelpTitle)
    delete vrHelpTitle;
  if(vrHelp)
    delete vrHelp;
}


PerformInteraction_request::PerformInteraction_request(const PerformInteraction_request& c)
{
  *this=c;
}


bool PerformInteraction_request::checkIntegrity(void)
{
  return PerformInteraction_requestMarshaller::checkIntegrity(*this);
}


PerformInteraction_request::PerformInteraction_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_PERFORMINTERACTION_REQUEST),
      helpPrompt(0),
    timeoutPrompt(0),
    timeout(0),
    vrHelpTitle(0),
    vrHelp(0)
{
}



bool PerformInteraction_request::set_initialText(const std::string& initialText_)
{
  if(initialText_.length()>500)  return false;
  initialText=initialText_;
  return true;
}

bool PerformInteraction_request::set_initialPrompt(const std::vector<TTSChunk>& initialPrompt_)
{
  unsigned int i=initialPrompt_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!TTSChunkMarshaller::checkIntegrityConst(initialPrompt_[i]))   return false;
  }
  initialPrompt=initialPrompt_;
  return true;
}

bool PerformInteraction_request::set_interactionMode(const InteractionMode& interactionMode_)
{
  if(!InteractionModeMarshaller::checkIntegrityConst(interactionMode_))   return false;
  interactionMode=interactionMode_;
  return true;
}

bool PerformInteraction_request::set_interactionChoiceSetIDList(const std::vector<unsigned int>& interactionChoiceSetIDList_)
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

bool PerformInteraction_request::set_helpPrompt(const std::vector<TTSChunk>& helpPrompt_)
{
  unsigned int i=helpPrompt_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!TTSChunkMarshaller::checkIntegrityConst(helpPrompt_[i]))   return false;
  }
  delete helpPrompt;
  helpPrompt=0;

  helpPrompt=new std::vector<TTSChunk>(helpPrompt_);
  return true;
}

void PerformInteraction_request::reset_helpPrompt(void)
{
  if(helpPrompt)
    delete helpPrompt;
  helpPrompt=0;
}

bool PerformInteraction_request::set_timeoutPrompt(const std::vector<TTSChunk>& timeoutPrompt_)
{
  unsigned int i=timeoutPrompt_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!TTSChunkMarshaller::checkIntegrityConst(timeoutPrompt_[i]))   return false;
  }
  delete timeoutPrompt;
  timeoutPrompt=0;

  timeoutPrompt=new std::vector<TTSChunk>(timeoutPrompt_);
  return true;
}

void PerformInteraction_request::reset_timeoutPrompt(void)
{
  if(timeoutPrompt)
    delete timeoutPrompt;
  timeoutPrompt=0;
}

bool PerformInteraction_request::set_timeout(unsigned int timeout_)
{
  if(timeout_>100000)  return false;
  if(timeout_<5000)  return false;
  delete timeout;
  timeout=0;

  timeout=new unsigned int(timeout_);
  return true;
}

void PerformInteraction_request::reset_timeout(void)
{
  if(timeout)
    delete timeout;
  timeout=0;
}

bool PerformInteraction_request::set_vrHelpTitle(const std::string& vrHelpTitle_)
{
  if(vrHelpTitle_.length()>500)  return false;
  delete vrHelpTitle;
  vrHelpTitle=0;

  vrHelpTitle=new std::string(vrHelpTitle_);
  return true;
}

void PerformInteraction_request::reset_vrHelpTitle(void)
{
  if(vrHelpTitle)
    delete vrHelpTitle;
  vrHelpTitle=0;
}

bool PerformInteraction_request::set_vrHelp(const std::vector<VrHelpItem>& vrHelp_)
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

void PerformInteraction_request::reset_vrHelp(void)
{
  if(vrHelp)
    delete vrHelp;
  vrHelp=0;
}




const std::string& PerformInteraction_request::get_initialText(void) const 
{
  return initialText;
}

const std::vector<TTSChunk>& PerformInteraction_request::get_initialPrompt(void) const 
{
  return initialPrompt;
}

const InteractionMode& PerformInteraction_request::get_interactionMode(void) const 
{
  return interactionMode;
}

const std::vector<unsigned int>& PerformInteraction_request::get_interactionChoiceSetIDList(void) const 
{
  return interactionChoiceSetIDList;
}

const std::vector<TTSChunk>* PerformInteraction_request::get_helpPrompt(void) const 
{
  return helpPrompt;
}

const std::vector<TTSChunk>* PerformInteraction_request::get_timeoutPrompt(void) const 
{
  return timeoutPrompt;
}

const unsigned int* PerformInteraction_request::get_timeout(void) const 
{
  return timeout;
}

const std::string* PerformInteraction_request::get_vrHelpTitle(void) const 
{
  return vrHelpTitle;
}

const std::vector<VrHelpItem>* PerformInteraction_request::get_vrHelp(void) const 
{
  return vrHelp;
}

