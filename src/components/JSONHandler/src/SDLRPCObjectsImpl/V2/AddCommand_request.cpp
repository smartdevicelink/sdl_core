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

#include "../include/JSONHandler/SDLRPCObjects/V2/AddCommand_request.h"
#include "AddCommand_requestMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Marshaller.h"
#include "ImageMarshaller.h"
#include "MenuParamsMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;
AddCommand_request& AddCommand_request::operator =(const AddCommand_request& c)
{
  cmdID= c.cmdID;
  menuParams= c.menuParams ? new MenuParams(c.menuParams[0]) : 0;
  vrCommands= c.vrCommands ? new std::vector<std::string>(c.vrCommands[0]) : 0;
  cmdIcon= c.cmdIcon ? new Image(c.cmdIcon[0]) : 0;

  return *this;
}


AddCommand_request::~AddCommand_request(void)
{
  if(menuParams)
    delete menuParams;
  if(vrCommands)
    delete vrCommands;
  if(cmdIcon)
    delete cmdIcon;
}


AddCommand_request::AddCommand_request(const AddCommand_request& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool AddCommand_request::checkIntegrity(void)
{
  return AddCommand_requestMarshaller::checkIntegrity(*this);
}


AddCommand_request::AddCommand_request(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION),
      menuParams(0),
    vrCommands(0),
    cmdIcon(0)
{
}



bool AddCommand_request::set_cmdID(unsigned int cmdID_)
{
  if(cmdID_>2000000000)  return false;
  cmdID=cmdID_;
  return true;
}

bool AddCommand_request::set_menuParams(const MenuParams& menuParams_)
{
  if(!MenuParamsMarshaller::checkIntegrityConst(menuParams_))   return false;
  delete menuParams;
  menuParams=0;

  menuParams=new MenuParams(menuParams_);
  return true;
}

void AddCommand_request::reset_menuParams(void)
{
  if(menuParams)
    delete menuParams;
  menuParams=0;
}

bool AddCommand_request::set_vrCommands(const std::vector<std::string>& vrCommands_)
{
  unsigned int i=vrCommands_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(vrCommands_[i].length()>99)  return false;
  }
  delete vrCommands;
  vrCommands=0;

  vrCommands=new std::vector<std::string>(vrCommands_);
  return true;
}

void AddCommand_request::reset_vrCommands(void)
{
  if(vrCommands)
    delete vrCommands;
  vrCommands=0;
}

bool AddCommand_request::set_cmdIcon(const Image& cmdIcon_)
{
  if(!ImageMarshaller::checkIntegrityConst(cmdIcon_))   return false;
  delete cmdIcon;
  cmdIcon=0;

  cmdIcon=new Image(cmdIcon_);
  return true;
}

void AddCommand_request::reset_cmdIcon(void)
{
  if(cmdIcon)
    delete cmdIcon;
  cmdIcon=0;
}




unsigned int AddCommand_request::get_cmdID(void) const
{
  return cmdID;
}

const MenuParams* AddCommand_request::get_menuParams(void) const 
{
  return menuParams;
}

const std::vector<std::string>* AddCommand_request::get_vrCommands(void) const 
{
  return vrCommands;
}

const Image* AddCommand_request::get_cmdIcon(void) const 
{
  return cmdIcon;
}

