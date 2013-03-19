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

#include "../include/JSONHandler/SDLRPCObjects/V2/ScrollableMessage_request.h"
#include "ScrollableMessage_requestMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Marshaller.h"
#include "SoftButtonMarshaller.h"

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
ScrollableMessage_request& ScrollableMessage_request::operator =(const ScrollableMessage_request& c)
{
  scrollableMessageBody= c.scrollableMessageBody;
  timeout= c.timeout ? new unsigned int(c.timeout[0]) : 0;
  softButtons= c.softButtons ? new std::vector<SoftButton>(c.softButtons[0]) : 0;

  return *this;
}


ScrollableMessage_request::~ScrollableMessage_request(void)
{
  if(timeout)
    delete timeout;
  if(softButtons)
    delete softButtons;
}


ScrollableMessage_request::ScrollableMessage_request(const ScrollableMessage_request& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool ScrollableMessage_request::checkIntegrity(void)
{
  return ScrollableMessage_requestMarshaller::checkIntegrity(*this);
}


ScrollableMessage_request::ScrollableMessage_request(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION),
      timeout(0),
    softButtons(0)
{
}



bool ScrollableMessage_request::set_scrollableMessageBody(const std::string& scrollableMessageBody_)
{
  if(scrollableMessageBody_.length()>500)  return false;
  scrollableMessageBody=scrollableMessageBody_;
  return true;
}

bool ScrollableMessage_request::set_timeout(unsigned int timeout_)
{
  if(timeout_>65535)  return false;
  delete timeout;
  timeout=0;

  timeout=new unsigned int(timeout_);
  return true;
}

void ScrollableMessage_request::reset_timeout(void)
{
  if(timeout)
    delete timeout;
  timeout=0;
}

bool ScrollableMessage_request::set_softButtons(const std::vector<SoftButton>& softButtons_)
{
  unsigned int i=softButtons_.size();
  if(i>8 || i<0)  return false;
  while(i--)
  {
    if(!SoftButtonMarshaller::checkIntegrityConst(softButtons_[i]))   return false;
  }
  delete softButtons;
  softButtons=0;

  softButtons=new std::vector<SoftButton>(softButtons_);
  return true;
}

void ScrollableMessage_request::reset_softButtons(void)
{
  if(softButtons)
    delete softButtons;
  softButtons=0;
}




const std::string& ScrollableMessage_request::get_scrollableMessageBody(void) const 
{
  return scrollableMessageBody;
}

const unsigned int* ScrollableMessage_request::get_timeout(void) const 
{
  return timeout;
}

const std::vector<SoftButton>* ScrollableMessage_request::get_softButtons(void) const 
{
  return softButtons;
}

