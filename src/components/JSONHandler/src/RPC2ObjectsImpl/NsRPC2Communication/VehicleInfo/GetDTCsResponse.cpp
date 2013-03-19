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

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/GetDTCsResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::VehicleInfo;


GetDTCsResponse& GetDTCsResponse::operator =(const GetDTCsResponse& c)
{
  if(dtcList)  delete dtcList;
  dtcList= c.dtcList ? new std::vector<NsSmartDeviceLinkRPCV2::DTC>(c.dtcList[0]) : 0;
  return *this;
}


GetDTCsResponse::~GetDTCsResponse(void)
{
  if(dtcList)  delete dtcList;
}


GetDTCsResponse::GetDTCsResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETDTCSRESPONSE),
  dtcList(0)
{
}


GetDTCsResponse::GetDTCsResponse(const GetDTCsResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETDTCSRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


const std::vector< NsSmartDeviceLinkRPCV2::DTC>* GetDTCsResponse::get_dtcList(void)
{
  return dtcList;
}

bool GetDTCsResponse::set_dtcList(const std::vector< NsSmartDeviceLinkRPCV2::DTC>& dtcList_)
{
  if(dtcList)  delete dtcList;
  dtcList=new std::vector< NsSmartDeviceLinkRPCV2::DTC>(dtcList_);
  return true;
}

void GetDTCsResponse::reset_dtcList(void)
{
  if(dtcList)  delete dtcList;
  dtcList=0;
}

bool GetDTCsResponse::checkIntegrity(void)
{
  return GetDTCsResponseMarshaller::checkIntegrity(*this);
}
