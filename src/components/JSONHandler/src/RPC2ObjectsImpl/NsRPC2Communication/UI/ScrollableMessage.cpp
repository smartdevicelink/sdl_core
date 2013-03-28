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

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/ScrollableMessage.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::UI;


ScrollableMessage& ScrollableMessage::operator =(const ScrollableMessage& c)
{
  scrollableMessageBody=c.scrollableMessageBody;
  if(timeout)  delete timeout;
  timeout= c.timeout ? new unsigned int(c.timeout[0]) : 0;
  if(softButtons)  delete softButtons;
  softButtons= c.softButtons ? new std::vector<NsSmartDeviceLinkRPCV2::SoftButton>(c.softButtons[0]) : 0;
  appId=c.appId;
  return *this;
}


ScrollableMessage::~ScrollableMessage(void)
{
  if(timeout)  delete timeout;
  if(softButtons)  delete softButtons;
}


ScrollableMessage::ScrollableMessage(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SCROLLABLEMESSAGE),
  timeout(0),
  softButtons(0)
{
}


ScrollableMessage::ScrollableMessage(const ScrollableMessage& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SCROLLABLEMESSAGE,c.getId())
{
  *this=c;
}


const std::string& ScrollableMessage::get_scrollableMessageBody(void)
{
  return scrollableMessageBody;
}

bool ScrollableMessage::set_scrollableMessageBody(const std::string& scrollableMessageBody_)
{
  scrollableMessageBody=scrollableMessageBody_;
  return true;
}

const unsigned int* ScrollableMessage::get_timeout(void)
{
  return timeout;
}

bool ScrollableMessage::set_timeout(const unsigned int& timeout_)
{
  if(timeout)  delete timeout;
  timeout=new unsigned int(timeout_);
  return true;
}

void ScrollableMessage::reset_timeout(void)
{
  if(timeout)  delete timeout;
  timeout=0;
}

const std::vector< NsSmartDeviceLinkRPCV2::SoftButton>* ScrollableMessage::get_softButtons(void)
{
  return softButtons;
}

bool ScrollableMessage::set_softButtons(const std::vector< NsSmartDeviceLinkRPCV2::SoftButton>& softButtons_)
{
  if(softButtons)  delete softButtons;
  softButtons=new std::vector< NsSmartDeviceLinkRPCV2::SoftButton>(softButtons_);
  return true;
}

void ScrollableMessage::reset_softButtons(void)
{
  if(softButtons)  delete softButtons;
  softButtons=0;
}

int ScrollableMessage::get_appId(void)
{
  return appId;
}

bool ScrollableMessage::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool ScrollableMessage::checkIntegrity(void)
{
  return ScrollableMessageMarshaller::checkIntegrity(*this);
}
