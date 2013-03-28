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

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Buttons/OnButtonEvent.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::Buttons
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::Buttons;


OnButtonEvent& OnButtonEvent::operator =(const OnButtonEvent& c)
{
  name=c.name;
  mode=c.mode;
  if(customButtonID)  delete customButtonID;
  customButtonID= c.customButtonID ? new unsigned int(c.customButtonID[0]) : 0;
  return *this;
}


OnButtonEvent::~OnButtonEvent(void)
{
  if(customButtonID)  delete customButtonID;
}


OnButtonEvent::OnButtonEvent(void) : 
  RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_BUTTONS__ONBUTTONEVENT),
  customButtonID(0)
{
}


OnButtonEvent::OnButtonEvent(const OnButtonEvent& c) : RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_BUTTONS__ONBUTTONEVENT)
{
  *this=c;
}


const NsSmartDeviceLinkRPCV2::ButtonName& OnButtonEvent::get_name(void)
{
  return name;
}

bool OnButtonEvent::set_name(const NsSmartDeviceLinkRPCV2::ButtonName& name_)
{
  name=name_;
  return true;
}

const NsSmartDeviceLinkRPCV2::ButtonEventMode& OnButtonEvent::get_mode(void)
{
  return mode;
}

bool OnButtonEvent::set_mode(const NsSmartDeviceLinkRPCV2::ButtonEventMode& mode_)
{
  mode=mode_;
  return true;
}

const unsigned int* OnButtonEvent::get_customButtonID(void)
{
  return customButtonID;
}

bool OnButtonEvent::set_customButtonID(const unsigned int& customButtonID_)
{
  if(customButtonID)  delete customButtonID;
  customButtonID=new unsigned int(customButtonID_);
  return true;
}

void OnButtonEvent::reset_customButtonID(void)
{
  if(customButtonID)  delete customButtonID;
  customButtonID=0;
}

bool OnButtonEvent::checkIntegrity(void)
{
  return OnButtonEventMarshaller::checkIntegrity(*this);
}
