#include "../include/JSONHandler/ALRPCObjects/V1/SetGlobalProperties_request.h"
#include "SetGlobalProperties_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V1/Marshaller.h"
#include "TTSChunkMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Dec 14 08:11:37 2012
  source stamp	Fri Dec 14 08:11:34 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;
SetGlobalProperties_request& SetGlobalProperties_request::operator =(const SetGlobalProperties_request& c)
{
  helpPrompt= c.helpPrompt ? new std::vector<TTSChunk>(c.helpPrompt[0]) : 0;
  timeoutPrompt= c.timeoutPrompt ? new std::vector<TTSChunk>(c.timeoutPrompt[0]) : 0;

  return *this;}


SetGlobalProperties_request::~SetGlobalProperties_request(void)
{
  if(helpPrompt)
    delete helpPrompt;
  if(timeoutPrompt)
    delete timeoutPrompt;
}


SetGlobalProperties_request::SetGlobalProperties_request(const SetGlobalProperties_request& c)
{
  *this=c;
}


bool SetGlobalProperties_request::checkIntegrity(void)
{
  return SetGlobalProperties_requestMarshaller::checkIntegrity(*this);
}


SetGlobalProperties_request::SetGlobalProperties_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_SETGLOBALPROPERTIES_REQUEST),
      helpPrompt(0),
    timeoutPrompt(0)
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




const std::vector<TTSChunk>* SetGlobalProperties_request::get_helpPrompt(void) const 
{
  return helpPrompt;
}

const std::vector<TTSChunk>* SetGlobalProperties_request::get_timeoutPrompt(void) const 
{
  return timeoutPrompt;
}

