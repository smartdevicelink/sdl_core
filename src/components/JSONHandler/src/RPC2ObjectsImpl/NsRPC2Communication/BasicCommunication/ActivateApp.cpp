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

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/BasicCommunication/ActivateApp.h"
#include "../src/../include/JSONHandler/RPC2Objects//Marshaller.h"

/*
  interface	NsRPC2Communication::SmartDeviceLinkCore
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::BasicCommunication;


ActivateApp& ActivateApp::operator =(const ActivateApp& c)
{
  appName=c.appName;
  appId=c.appId;
  return *this;
}


ActivateApp::~ActivateApp(void)
{
}


ActivateApp::ActivateApp(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_BASICCOMMUNICATION__ACTIVATEAPP)
{
}


ActivateApp::ActivateApp(const ActivateApp& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_BASICCOMMUNICATION__ACTIVATEAPP,c.getId())
{
  *this=c;
}


const std::string& ActivateApp::get_appName(void)
{
  return appName;
}

bool ActivateApp::set_appName(const std::string& appName_)
{
  appName=appName_;
  return true;
}

int ActivateApp::get_appId(void)
{
  return appId;
}

bool ActivateApp::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool ActivateApp::checkIntegrity(void)
{
  return ActivateAppMarshaller::checkIntegrity(*this);
}
