//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/SetGlobalProperties.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::UI;


SetGlobalProperties& SetGlobalProperties::operator =(const SetGlobalProperties& c)
{
  if(helpPrompt)  delete helpPrompt;
  helpPrompt= c.helpPrompt ? new std::vector<NsSmartDeviceLinkRPCV2::TTSChunk>(c.helpPrompt[0]) : 0;
  if(timeoutPrompt)  delete timeoutPrompt;
  timeoutPrompt= c.timeoutPrompt ? new std::vector<NsSmartDeviceLinkRPCV2::TTSChunk>(c.timeoutPrompt[0]) : 0;
  if(vrHelpTitle)  delete vrHelpTitle;
  vrHelpTitle= c.vrHelpTitle ? new std::string(c.vrHelpTitle[0]) : 0;
  if(vrHelp)  delete vrHelp;
  vrHelp= c.vrHelp ? new std::vector<NsSmartDeviceLinkRPCV2::VrHelpItem>(c.vrHelp[0]) : 0;
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


const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>* SetGlobalProperties::get_helpPrompt(void)
{
  return helpPrompt;
}

bool SetGlobalProperties::set_helpPrompt(const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>& helpPrompt_)
{
  if(helpPrompt)  delete helpPrompt;
  helpPrompt=new std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>(helpPrompt_);
  return true;
}

void SetGlobalProperties::reset_helpPrompt(void)
{
  if(helpPrompt)  delete helpPrompt;
  helpPrompt=0;
}

const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>* SetGlobalProperties::get_timeoutPrompt(void)
{
  return timeoutPrompt;
}

bool SetGlobalProperties::set_timeoutPrompt(const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>& timeoutPrompt_)
{
  if(timeoutPrompt)  delete timeoutPrompt;
  timeoutPrompt=new std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>(timeoutPrompt_);
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

const std::vector< NsSmartDeviceLinkRPCV2::VrHelpItem>* SetGlobalProperties::get_vrHelp(void)
{
  return vrHelp;
}

bool SetGlobalProperties::set_vrHelp(const std::vector< NsSmartDeviceLinkRPCV2::VrHelpItem>& vrHelp_)
{
  if(vrHelp)  delete vrHelp;
  vrHelp=new std::vector< NsSmartDeviceLinkRPCV2::VrHelpItem>(vrHelp_);
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
