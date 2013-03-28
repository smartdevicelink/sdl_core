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

#include "../include/JSONHandler/SDLRPCObjects/V1/AddSubMenu_request.h"
#include "AddSubMenu_requestMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V1/Marshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPC;
AddSubMenu_request& AddSubMenu_request::operator =(const AddSubMenu_request& c)
{
  menuID= c.menuID;
  position= c.position ? new unsigned int(c.position[0]) : 0;
  menuName= c.menuName;

  return *this;}


AddSubMenu_request::~AddSubMenu_request(void)
{
  if(position)
    delete position;
}


AddSubMenu_request::AddSubMenu_request(const AddSubMenu_request& c)
{
  *this=c;
}


bool AddSubMenu_request::checkIntegrity(void)
{
  return AddSubMenu_requestMarshaller::checkIntegrity(*this);
}


AddSubMenu_request::AddSubMenu_request(void) : SDLRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_ADDSUBMENU_REQUEST),
      position(0)
{
}



bool AddSubMenu_request::set_menuID(unsigned int menuID_)
{
  if(menuID_>2000000000)  return false;
  menuID=menuID_;
  return true;
}

bool AddSubMenu_request::set_position(unsigned int position_)
{
  if(position_>1000)  return false;
  delete position;
  position=0;

  position=new unsigned int(position_);
  return true;
}

void AddSubMenu_request::reset_position(void)
{
  if(position)
    delete position;
  position=0;
}

bool AddSubMenu_request::set_menuName(const std::string& menuName_)
{
  if(menuName_.length()>500)  return false;
  menuName=menuName_;
  return true;
}




unsigned int AddSubMenu_request::get_menuID(void) const
{
  return menuID;
}

const unsigned int* AddSubMenu_request::get_position(void) const 
{
  return position;
}

const std::string& AddSubMenu_request::get_menuName(void) const 
{
  return menuName;
}

