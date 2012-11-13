#include "../include/JSONHandler/RPC2Objects/UI/SetGlobalProperties.h"
#include "../include/JSONHandler/RPC2Objects/UI/Marshaller.h"

/*
  interface	RPC2Communication::UI
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::UI;


SetGlobalProperties& SetGlobalProperties::operator =(const SetGlobalProperties& c)
{
  if(helpPrompt)  delete helpPrompt;
  helpPrompt= c.helpPrompt ? new std::vector<TTSChunk>(c.helpPrompt[0]) : 0;
  if(timeoutPrompt)  delete timeoutPrompt;
  timeoutPrompt= c.timeoutPrompt ? new std::vector<TTSChunk>(c.timeoutPrompt[0]) : 0;
  return *this;
}


SetGlobalProperties::~SetGlobalProperties(void)
{
  if(helpPrompt)  delete helpPrompt;
  if(timeoutPrompt)  delete timeoutPrompt;
}


SetGlobalProperties::SetGlobalProperties(void) : 
  RPC2Request(Marshaller::METHOD_SETGLOBALPROPERTIES),
  helpPrompt(0),
  timeoutPrompt(0)
{
}


SetGlobalProperties::SetGlobalProperties(const SetGlobalProperties& c) : RPC2Request(Marshaller::METHOD_SETGLOBALPROPERTIES,c.getId())
{
  *this=c;
}


const std::vector< TTSChunk>* SetGlobalProperties::get_helpPrompt(void)
{
  return helpPrompt;
}

bool SetGlobalProperties::set_helpPrompt(const std::vector< TTSChunk>& helpPrompt_)
{
  if(helpPrompt)  delete helpPrompt;
  helpPrompt=new std::vector< TTSChunk>(helpPrompt_);
  return true;
}

void SetGlobalProperties::reset_helpPrompt(void)
{
  if(helpPrompt)  delete helpPrompt;
  helpPrompt=0;
}

const std::vector< TTSChunk>* SetGlobalProperties::get_timeoutPrompt(void)
{
  return timeoutPrompt;
}

bool SetGlobalProperties::set_timeoutPrompt(const std::vector< TTSChunk>& timeoutPrompt_)
{
  if(timeoutPrompt)  delete timeoutPrompt;
  timeoutPrompt=new std::vector< TTSChunk>(timeoutPrompt_);
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
