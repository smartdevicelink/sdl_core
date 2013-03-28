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

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/ReadDID.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::VehicleInfo;


ReadDID& ReadDID::operator =(const ReadDID& c)
{
  ecuName=c.ecuName;
  didLocation=c.didLocation;
  if(encrypted)  delete encrypted;
  encrypted= c.encrypted ? new bool(c.encrypted[0]) : 0;
  appId=c.appId;
  return *this;
}


ReadDID::~ReadDID(void)
{
  if(encrypted)  delete encrypted;
}


ReadDID::ReadDID(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__READDID),
  encrypted(0)
{
}


ReadDID::ReadDID(const ReadDID& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__READDID,c.getId())
{
  *this=c;
}


unsigned int ReadDID::get_ecuName(void)
{
  return ecuName;
}

bool ReadDID::set_ecuName(unsigned int ecuName_)
{
  ecuName=ecuName_;
  return true;
}

const std::vector< unsigned int>& ReadDID::get_didLocation(void)
{
  return didLocation;
}

bool ReadDID::set_didLocation(const std::vector< unsigned int>& didLocation_)
{
  didLocation=didLocation_;
  return true;
}

const bool* ReadDID::get_encrypted(void)
{
  return encrypted;
}

bool ReadDID::set_encrypted(const bool& encrypted_)
{
  if(encrypted)  delete encrypted;
  encrypted=new bool(encrypted_);
  return true;
}

void ReadDID::reset_encrypted(void)
{
  if(encrypted)  delete encrypted;
  encrypted=0;
}

int ReadDID::get_appId(void)
{
  return appId;
}

bool ReadDID::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool ReadDID::checkIntegrity(void)
{
  return ReadDIDMarshaller::checkIntegrity(*this);
}
