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

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/ReadDIDResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::VehicleInfo;


ReadDIDResponse& ReadDIDResponse::operator =(const ReadDIDResponse& c)
{
  if(dataResult)  delete dataResult;
  dataResult= c.dataResult ? new std::vector<NsSmartDeviceLinkRPCV2::VehicleDataResultCode>(c.dataResult[0]) : 0;
  if(data)  delete data;
  data= c.data ? new std::vector<std::string>(c.data[0]) : 0;
  return *this;
}


ReadDIDResponse::~ReadDIDResponse(void)
{
  if(dataResult)  delete dataResult;
  if(data)  delete data;
}


ReadDIDResponse::ReadDIDResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__READDIDRESPONSE),
  dataResult(0),
  data(0)
{
}


ReadDIDResponse::ReadDIDResponse(const ReadDIDResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__READDIDRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


const std::vector< NsSmartDeviceLinkRPCV2::VehicleDataResultCode>* ReadDIDResponse::get_dataResult(void)
{
  return dataResult;
}

bool ReadDIDResponse::set_dataResult(const std::vector< NsSmartDeviceLinkRPCV2::VehicleDataResultCode>& dataResult_)
{
  if(dataResult)  delete dataResult;
  dataResult=new std::vector< NsSmartDeviceLinkRPCV2::VehicleDataResultCode>(dataResult_);
  return true;
}

void ReadDIDResponse::reset_dataResult(void)
{
  if(dataResult)  delete dataResult;
  dataResult=0;
}

const std::vector< std::string>* ReadDIDResponse::get_data(void)
{
  return data;
}

bool ReadDIDResponse::set_data(const std::vector< std::string>& data_)
{
  if(data)  delete data;
  data=new std::vector< std::string>(data_);
  return true;
}

void ReadDIDResponse::reset_data(void)
{
  if(data)  delete data;
  data=0;
}

bool ReadDIDResponse::checkIntegrity(void)
{
  return ReadDIDResponseMarshaller::checkIntegrity(*this);
}
