#include "../include/JSONHandler/ALRPCObjects/SetGlobalProperties_v2_request.h"
#include "SetGlobalProperties_v2_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
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
SetGlobalProperties_v2_request& SetGlobalProperties_v2_request::operator =(const SetGlobalProperties_v2_request& c)
{
  helpPrompt= c.helpPrompt ? new std::vector<TTSChunk_v2>(c.helpPrompt[0]) : 0;
  timeoutPrompt= c.timeoutPrompt ? new std::vector<TTSChunk_v2>(c.timeoutPrompt[0]) : 0;
  vrHelpTitle= c.vrHelpTitle ? new std::string(c.vrHelpTitle[0]) : 0;
  vrHelp= c.vrHelp ? new std::vector<VrHelpItem>(c.vrHelp[0]) : 0;

  return *this;}


SetGlobalProperties_v2_request::~SetGlobalProperties_v2_request(void)
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


SetGlobalProperties_v2_request::SetGlobalProperties_v2_request(const SetGlobalProperties_v2_request& c)
{
  *this=c;
}


bool SetGlobalProperties_v2_request::checkIntegrity(void)
{
  return SetGlobalProperties_v2_requestMarshaller::checkIntegrity(*this);
}


SetGlobalProperties_v2_request::SetGlobalProperties_v2_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_SETGLOBALPROPERTIES_V2_REQUEST),
      helpPrompt(0),
    timeoutPrompt(0),
    vrHelpTitle(0),
    vrHelp(0)
{
}



bool SetGlobalProperties_v2_request::set_helpPrompt(const std::vector<TTSChunk_v2>& helpPrompt_)
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

void SetGlobalProperties_v2_request::reset_helpPrompt(void)
{
  if(helpPrompt)
    delete helpPrompt;
  helpPrompt=0;
}

bool SetGlobalProperties_v2_request::set_timeoutPrompt(const std::vector<TTSChunk_v2>& timeoutPrompt_)
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

void SetGlobalProperties_v2_request::reset_timeoutPrompt(void)
{
  if(timeoutPrompt)
    delete timeoutPrompt;
  timeoutPrompt=0;
}

bool SetGlobalProperties_v2_request::set_vrHelpTitle(const std::string& vrHelpTitle_)
{
  if(vrHelpTitle_.length()>500)  return false;
  delete vrHelpTitle;
  vrHelpTitle=0;

  vrHelpTitle=new std::string(vrHelpTitle_);
  return true;
}

void SetGlobalProperties_v2_request::reset_vrHelpTitle(void)
{
  if(vrHelpTitle)
    delete vrHelpTitle;
  vrHelpTitle=0;
}

bool SetGlobalProperties_v2_request::set_vrHelp(const std::vector<VrHelpItem>& vrHelp_)
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

void SetGlobalProperties_v2_request::reset_vrHelp(void)
{
  if(vrHelp)
    delete vrHelp;
  vrHelp=0;
}




const std::vector<TTSChunk_v2>* SetGlobalProperties_v2_request::get_helpPrompt(void) const 
{
  return helpPrompt;
}

const std::vector<TTSChunk_v2>* SetGlobalProperties_v2_request::get_timeoutPrompt(void) const 
{
  return timeoutPrompt;
}

const std::string* SetGlobalProperties_v2_request::get_vrHelpTitle(void) const 
{
  return vrHelpTitle;
}

const std::vector<VrHelpItem>* SetGlobalProperties_v2_request::get_vrHelp(void) const 
{
  return vrHelp;
}

