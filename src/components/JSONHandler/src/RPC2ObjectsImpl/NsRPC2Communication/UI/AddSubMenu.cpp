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

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/AddSubMenu.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::UI;


AddSubMenu& AddSubMenu::operator =(const AddSubMenu& c)
{
  menuId=c.menuId;
  if(position)  delete position;
  position= c.position ? new unsigned int(c.position[0]) : 0;
  menuName=c.menuName;
  appId=c.appId;
  return *this;
}


AddSubMenu::~AddSubMenu(void)
{
  if(position)  delete position;
}


AddSubMenu::AddSubMenu(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ADDSUBMENU),
  position(0)
{
}


AddSubMenu::AddSubMenu(const AddSubMenu& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ADDSUBMENU,c.getId())
{
  *this=c;
}


unsigned int AddSubMenu::get_menuId(void)
{
  return menuId;
}

bool AddSubMenu::set_menuId(unsigned int menuId_)
{
  menuId=menuId_;
  return true;
}

const unsigned int* AddSubMenu::get_position(void)
{
  return position;
}

bool AddSubMenu::set_position(const unsigned int& position_)
{
  if(position)  delete position;
  position=new unsigned int(position_);
  return true;
}

void AddSubMenu::reset_position(void)
{
  if(position)  delete position;
  position=0;
}

const std::string& AddSubMenu::get_menuName(void)
{
  return menuName;
}

bool AddSubMenu::set_menuName(const std::string& menuName_)
{
  menuName=menuName_;
  return true;
}

int AddSubMenu::get_appId(void)
{
  return appId;
}

bool AddSubMenu::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool AddSubMenu::checkIntegrity(void)
{
  return AddSubMenuMarshaller::checkIntegrity(*this);
}
