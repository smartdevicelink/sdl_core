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

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/GetCapabilitiesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::UI;


GetCapabilitiesResponse& GetCapabilitiesResponse::operator =(const GetCapabilitiesResponse& c)
{
  displayCapabilities=c.displayCapabilities;
  hmiZoneCapabilities=c.hmiZoneCapabilities;
  if(softButtonCapabilities)  delete softButtonCapabilities;
  softButtonCapabilities= c.softButtonCapabilities ? new std::vector<NsSmartDeviceLinkRPCV2::SoftButtonCapabilities>(c.softButtonCapabilities[0]) : 0;
  return *this;
}


GetCapabilitiesResponse::~GetCapabilitiesResponse(void)
{
  if(softButtonCapabilities)  delete softButtonCapabilities;
}


GetCapabilitiesResponse::GetCapabilitiesResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__GETCAPABILITIESRESPONSE),
  softButtonCapabilities(0)
{
}


GetCapabilitiesResponse::GetCapabilitiesResponse(const GetCapabilitiesResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__GETCAPABILITIESRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


const NsSmartDeviceLinkRPCV2::DisplayCapabilities& GetCapabilitiesResponse::get_displayCapabilities(void)
{
  return displayCapabilities;
}

bool GetCapabilitiesResponse::set_displayCapabilities(const NsSmartDeviceLinkRPCV2::DisplayCapabilities& displayCapabilities_)
{
  displayCapabilities=displayCapabilities_;
  return true;
}

const std::vector< NsSmartDeviceLinkRPCV2::HmiZoneCapabilities>& GetCapabilitiesResponse::get_hmiZoneCapabilities(void)
{
  return hmiZoneCapabilities;
}

bool GetCapabilitiesResponse::set_hmiZoneCapabilities(const std::vector< NsSmartDeviceLinkRPCV2::HmiZoneCapabilities>& hmiZoneCapabilities_)
{
  hmiZoneCapabilities=hmiZoneCapabilities_;
  return true;
}

const std::vector< NsSmartDeviceLinkRPCV2::SoftButtonCapabilities>* GetCapabilitiesResponse::get_softButtonCapabilities(void)
{
  return softButtonCapabilities;
}

bool GetCapabilitiesResponse::set_softButtonCapabilities(const std::vector< NsSmartDeviceLinkRPCV2::SoftButtonCapabilities>& softButtonCapabilities_)
{
  if(softButtonCapabilities)  delete softButtonCapabilities;
  softButtonCapabilities=new std::vector< NsSmartDeviceLinkRPCV2::SoftButtonCapabilities>(softButtonCapabilities_);
  return true;
}

void GetCapabilitiesResponse::reset_softButtonCapabilities(void)
{
  if(softButtonCapabilities)  delete softButtonCapabilities;
  softButtonCapabilities=0;
}

bool GetCapabilitiesResponse::checkIntegrity(void)
{
  return GetCapabilitiesResponseMarshaller::checkIntegrity(*this);
}
