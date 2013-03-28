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

#include "../include/JSONHandler/RPC2Objects/NsRPC2Communication/BasicCommunication/SendData.h"
#include "../include/JSONHandler/RPC2Objects//Marshaller.h"

/*
  interface	NsRPC2Communication::BasicCommunication
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::BasicCommunication;


SendData& SendData::operator =(const SendData& c)
{
  data=c.data;
  if(url)  delete url;
  url= c.url ? new std::string(c.url[0]) : 0;
  if(timeout)  delete timeout;
  timeout= c.timeout ? new int(c.timeout[0]) : 0;
  return *this;
}


SendData::~SendData(void)
{
  if(url)  delete url;
  if(timeout)  delete timeout;
}


SendData::SendData(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_BASICCOMMUNICATION__SENDDATA),
  url(0),
  timeout(0)
{
}


SendData::SendData(const SendData& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_BASICCOMMUNICATION__SENDDATA,c.getId())
{
  *this=c;
}


const std::vector< std::string>& SendData::get_data(void)
{
  return data;
}

bool SendData::set_data(const std::vector< std::string>& data_)
{
  data=data_;
  return true;
}

const std::string* SendData::get_url(void)
{
  return url;
}

bool SendData::set_url(const std::string& url_)
{
  if(url)  delete url;
  url=new std::string(url_);
  return true;
}

void SendData::reset_url(void)
{
  if(url)  delete url;
  url=0;
}

const int* SendData::get_timeout(void)
{
  return timeout;
}

bool SendData::set_timeout(const int& timeout_)
{
  if(timeout)  delete timeout;
  timeout=new int(timeout_);
  return true;
}

void SendData::reset_timeout(void)
{
  if(timeout)  delete timeout;
  timeout=0;
}

bool SendData::checkIntegrity(void)
{
  return SendDataMarshaller::checkIntegrity(*this);
}
