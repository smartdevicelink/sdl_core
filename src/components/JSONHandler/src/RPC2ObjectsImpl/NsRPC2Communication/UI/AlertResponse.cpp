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

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/AlertResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::UI;


AlertResponse& AlertResponse::operator =(const AlertResponse& c)
{
  if(tryAgainTime)  delete tryAgainTime;
  tryAgainTime= c.tryAgainTime ? new unsigned int(c.tryAgainTime[0]) : 0;
  return *this;
}


AlertResponse::~AlertResponse(void)
{
  if(tryAgainTime)  delete tryAgainTime;
}


AlertResponse::AlertResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ALERTRESPONSE),
  tryAgainTime(0)
{
}


AlertResponse::AlertResponse(const AlertResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ALERTRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


const unsigned int* AlertResponse::get_tryAgainTime(void)
{
  return tryAgainTime;
}

bool AlertResponse::set_tryAgainTime(const unsigned int& tryAgainTime_)
{
  if(tryAgainTime)  delete tryAgainTime;
  tryAgainTime=new unsigned int(tryAgainTime_);
  return true;
}

void AlertResponse::reset_tryAgainTime(void)
{
  if(tryAgainTime)  delete tryAgainTime;
  tryAgainTime=0;
}

bool AlertResponse::checkIntegrity(void)
{
  return AlertResponseMarshaller::checkIntegrity(*this);
}
