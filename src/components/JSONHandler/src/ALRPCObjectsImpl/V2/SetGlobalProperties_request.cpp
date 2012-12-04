#include "../include/JSONHandler/ALRPCObjects/V2/SetGlobalProperties_request.h"
#include "SetGlobalProperties_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "TTSChunkMarshaller.h"
#include "VrHelpItemMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;
SetGlobalProperties_request& SetGlobalProperties_request::operator =(const SetGlobalProperties_request& c)
{
  helpPrompt= c.helpPrompt ? new std::vector<TTSChunk>(c.helpPrompt[0]) : 0;
  timeoutPrompt= c.timeoutPrompt ? new std::vector<TTSChunk>(c.timeoutPrompt[0]) : 0;
  vrHelpTitle= c.vrHelpTitle ? new std::string(c.vrHelpTitle[0]) : 0;
  vrHelp= c.vrHelp ? new std::vector<VrHelpItem>(c.vrHelp[0]) : 0;

  return *this;
}


SetGlobalProperties_request::~SetGlobalProperties_request(void)
{
  if(helpPrompt)
    delete helpPrompt;
  if(timeoutPrompt)
    delete timeoutPrompt;
  if(vrHelpTitle)
    delete vrHelpTitle;
  if(vrHelp)
    delete vrHelp;
}


SetGlobalProperties_request::SetGlobalProperties_request(const SetGlobalProperties_request& c) : ALRPC2Message(c)
{
  *this=c;
}


bool SetGlobalProperties_request::checkIntegrity(void)
{
  return SetGlobalProperties_requestMarshaller::checkIntegrity(*this);
}


SetGlobalProperties_request::SetGlobalProperties_request(void) : ALRPC2Message(PROTOCOL_VERSION),
      helpPrompt(0),
    timeoutPrompt(0),
    vrHelpTitle(0),
    vrHelp(0)
{
}



bool SetGlobalProperties_request::set_helpPrompt(const std::vector<TTSChunk>& helpPrompt_)
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

void SetGlobalProperties_request::reset_helpPrompt(void)
{
  if(helpPrompt)
    delete helpPrompt;
  helpPrompt=0;
}

bool SetGlobalProperties_request::set_timeoutPrompt(const std::vector<TTSChunk>& timeoutPrompt_)
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

void SetGlobalProperties_request::reset_timeoutPrompt(void)
{
  if(timeoutPrompt)
    delete timeoutPrompt;
  timeoutPrompt=0;
}

bool SetGlobalProperties_request::set_vrHelpTitle(const std::string& vrHelpTitle_)
{
  if(vrHelpTitle_.length()>500)  return false;
  delete vrHelpTitle;
  vrHelpTitle=0;

  vrHelpTitle=new std::string(vrHelpTitle_);
  return true;
}

void SetGlobalProperties_request::reset_vrHelpTitle(void)
{
  if(vrHelpTitle)
    delete vrHelpTitle;
  vrHelpTitle=0;
}

bool SetGlobalProperties_request::set_vrHelp(const std::vector<VrHelpItem>& vrHelp_)
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

void SetGlobalProperties_request::reset_vrHelp(void)
{
  if(vrHelp)
    delete vrHelp;
  vrHelp=0;
}




const std::vector<TTSChunk>* SetGlobalProperties_request::get_helpPrompt(void) const 
{
  return helpPrompt;
}

const std::vector<TTSChunk>* SetGlobalProperties_request::get_timeoutPrompt(void) const 
{
  return timeoutPrompt;
}

const std::string* SetGlobalProperties_request::get_vrHelpTitle(void) const 
{
  return vrHelpTitle;
}

const std::vector<VrHelpItem>* SetGlobalProperties_request::get_vrHelp(void) const 
{
  return vrHelp;
}

