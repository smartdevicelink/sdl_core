/**
* \file RPC2Response.cpp
* \brief RPC2Response class source file.
* Copyright (c) 2013, Ford Motor Company
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
*/


#include "JSONHandler/RPC2Response.h"

using namespace NsRPC2Communication;


RPC2Response::RPC2Response() :
RPC2Command(RESPONSE)
, mId(0)
, mResultCode(-1)
{
}

RPC2Response::RPC2Response(int method) :
RPC2Command(RESPONSE, method)
, mId(0)
, mResultCode(-1)
{
}


RPC2Response::RPC2Response(int method,unsigned int id) :
RPC2Command(RESPONSE, method)
, mId(id)
, mResultCode(-1)
{
}


RPC2Response::RPC2Response(int method,unsigned int id,int res) :
RPC2Command(RESPONSE, method)
, mId(id)
, mResultCode(res)
{
}


RPC2Response::~RPC2Response()
{
}

void RPC2Response::setId(unsigned int id)
{
  mId=id;
}

unsigned int RPC2Response::getId() const
{
  return mId;
}


int RPC2Response::getResult() const
{
  return mResultCode;
}

void RPC2Response::setResult(int r)
{
  mResultCode=r;
}
