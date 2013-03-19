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

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/ShowConstantTBT.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::UI;


ShowConstantTBT& ShowConstantTBT::operator =(const ShowConstantTBT& c)
{
  if(navigationText1)  delete navigationText1;
  navigationText1= c.navigationText1 ? new std::string(c.navigationText1[0]) : 0;
  if(navigationText2)  delete navigationText2;
  navigationText2= c.navigationText2 ? new std::string(c.navigationText2[0]) : 0;
  if(eta)  delete eta;
  eta= c.eta ? new std::string(c.eta[0]) : 0;
  if(totalDistance)  delete totalDistance;
  totalDistance= c.totalDistance ? new std::string(c.totalDistance[0]) : 0;
  turnIcon=c.turnIcon;
  distanceToManeuver=c.distanceToManeuver;
  distanceToManeuverScale=c.distanceToManeuverScale;
  if(maneuverComplete)  delete maneuverComplete;
  maneuverComplete= c.maneuverComplete ? new bool(c.maneuverComplete[0]) : 0;
  softButtons=c.softButtons;
  appId=c.appId;
  return *this;
}


ShowConstantTBT::~ShowConstantTBT(void)
{
  if(navigationText1)  delete navigationText1;
  if(navigationText2)  delete navigationText2;
  if(eta)  delete eta;
  if(totalDistance)  delete totalDistance;
  if(maneuverComplete)  delete maneuverComplete;
}


ShowConstantTBT::ShowConstantTBT(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SHOWCONSTANTTBT),
  navigationText1(0),
  navigationText2(0),
  eta(0),
  totalDistance(0),
  maneuverComplete(0)
{
}


ShowConstantTBT::ShowConstantTBT(const ShowConstantTBT& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SHOWCONSTANTTBT,c.getId())
{
  *this=c;
}


const std::string* ShowConstantTBT::get_navigationText1(void)
{
  return navigationText1;
}

bool ShowConstantTBT::set_navigationText1(const std::string& navigationText1_)
{
  if(navigationText1)  delete navigationText1;
  navigationText1=new std::string(navigationText1_);
  return true;
}

void ShowConstantTBT::reset_navigationText1(void)
{
  if(navigationText1)  delete navigationText1;
  navigationText1=0;
}

const std::string* ShowConstantTBT::get_navigationText2(void)
{
  return navigationText2;
}

bool ShowConstantTBT::set_navigationText2(const std::string& navigationText2_)
{
  if(navigationText2)  delete navigationText2;
  navigationText2=new std::string(navigationText2_);
  return true;
}

void ShowConstantTBT::reset_navigationText2(void)
{
  if(navigationText2)  delete navigationText2;
  navigationText2=0;
}

const std::string* ShowConstantTBT::get_eta(void)
{
  return eta;
}

bool ShowConstantTBT::set_eta(const std::string& eta_)
{
  if(eta)  delete eta;
  eta=new std::string(eta_);
  return true;
}

void ShowConstantTBT::reset_eta(void)
{
  if(eta)  delete eta;
  eta=0;
}

const std::string* ShowConstantTBT::get_totalDistance(void)
{
  return totalDistance;
}

bool ShowConstantTBT::set_totalDistance(const std::string& totalDistance_)
{
  if(totalDistance)  delete totalDistance;
  totalDistance=new std::string(totalDistance_);
  return true;
}

void ShowConstantTBT::reset_totalDistance(void)
{
  if(totalDistance)  delete totalDistance;
  totalDistance=0;
}

const NsSmartDeviceLinkRPCV2::Image& ShowConstantTBT::get_turnIcon(void)
{
  return turnIcon;
}

bool ShowConstantTBT::set_turnIcon(const NsSmartDeviceLinkRPCV2::Image& turnIcon_)
{
  turnIcon=turnIcon_;
  return true;
}

float ShowConstantTBT::get_distanceToManeuver(void)
{
  return distanceToManeuver;
}

bool ShowConstantTBT::set_distanceToManeuver(float distanceToManeuver_)
{
  distanceToManeuver=distanceToManeuver_;
  return true;
}

float ShowConstantTBT::get_distanceToManeuverScale(void)
{
  return distanceToManeuverScale;
}

bool ShowConstantTBT::set_distanceToManeuverScale(float distanceToManeuverScale_)
{
  distanceToManeuverScale=distanceToManeuverScale_;
  return true;
}

const bool* ShowConstantTBT::get_maneuverComplete(void)
{
  return maneuverComplete;
}

bool ShowConstantTBT::set_maneuverComplete(const bool& maneuverComplete_)
{
  if(maneuverComplete)  delete maneuverComplete;
  maneuverComplete=new bool(maneuverComplete_);
  return true;
}

void ShowConstantTBT::reset_maneuverComplete(void)
{
  if(maneuverComplete)  delete maneuverComplete;
  maneuverComplete=0;
}

const std::vector< NsSmartDeviceLinkRPCV2::SoftButton>& ShowConstantTBT::get_softButtons(void)
{
  return softButtons;
}

bool ShowConstantTBT::set_softButtons(const std::vector< NsSmartDeviceLinkRPCV2::SoftButton>& softButtons_)
{
  softButtons=softButtons_;
  return true;
}

int ShowConstantTBT::get_appId(void)
{
  return appId;
}

bool ShowConstantTBT::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool ShowConstantTBT::checkIntegrity(void)
{
  return ShowConstantTBTMarshaller::checkIntegrity(*this);
}
