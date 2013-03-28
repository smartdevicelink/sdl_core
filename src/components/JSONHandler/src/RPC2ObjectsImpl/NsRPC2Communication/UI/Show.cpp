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

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/Show.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::UI;


Show& Show::operator =(const Show& c)
{
  if(mainField1)  delete mainField1;
  mainField1= c.mainField1 ? new std::string(c.mainField1[0]) : 0;
  if(mainField2)  delete mainField2;
  mainField2= c.mainField2 ? new std::string(c.mainField2[0]) : 0;
  if(mainField3)  delete mainField3;
  mainField3= c.mainField3 ? new std::string(c.mainField3[0]) : 0;
  if(mainField4)  delete mainField4;
  mainField4= c.mainField4 ? new std::string(c.mainField4[0]) : 0;
  if(alignment)  delete alignment;
  alignment= c.alignment ? new NsSmartDeviceLinkRPCV2::TextAlignment(c.alignment[0]) : 0;
  if(statusBar)  delete statusBar;
  statusBar= c.statusBar ? new std::string(c.statusBar[0]) : 0;
  if(mediaClock)  delete mediaClock;
  mediaClock= c.mediaClock ? new std::string(c.mediaClock[0]) : 0;
  if(mediaTrack)  delete mediaTrack;
  mediaTrack= c.mediaTrack ? new std::string(c.mediaTrack[0]) : 0;
  if(graphic)  delete graphic;
  graphic= c.graphic ? new NsSmartDeviceLinkRPCV2::Image(c.graphic[0]) : 0;
  if(softButtons)  delete softButtons;
  softButtons= c.softButtons ? new std::vector<NsSmartDeviceLinkRPCV2::SoftButton>(c.softButtons[0]) : 0;
  if(customPresets)  delete customPresets;
  customPresets= c.customPresets ? new std::vector<std::string>(c.customPresets[0]) : 0;
  appId=c.appId;
  return *this;
}


Show::~Show(void)
{
  if(mainField1)  delete mainField1;
  if(mainField2)  delete mainField2;
  if(mainField3)  delete mainField3;
  if(mainField4)  delete mainField4;
  if(alignment)  delete alignment;
  if(statusBar)  delete statusBar;
  if(mediaClock)  delete mediaClock;
  if(mediaTrack)  delete mediaTrack;
  if(graphic)  delete graphic;
  if(softButtons)  delete softButtons;
  if(customPresets)  delete customPresets;
}


Show::Show(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SHOW),
  mainField1(0),
  mainField2(0),
  mainField3(0),
  mainField4(0),
  alignment(0),
  statusBar(0),
  mediaClock(0),
  mediaTrack(0),
  graphic(0),
  softButtons(0),
  customPresets(0)
{
}


Show::Show(const Show& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SHOW,c.getId())
{
  *this=c;
}


const std::string* Show::get_mainField1(void)
{
  return mainField1;
}

bool Show::set_mainField1(const std::string& mainField1_)
{
  if(mainField1)  delete mainField1;
  mainField1=new std::string(mainField1_);
  return true;
}

void Show::reset_mainField1(void)
{
  if(mainField1)  delete mainField1;
  mainField1=0;
}

const std::string* Show::get_mainField2(void)
{
  return mainField2;
}

bool Show::set_mainField2(const std::string& mainField2_)
{
  if(mainField2)  delete mainField2;
  mainField2=new std::string(mainField2_);
  return true;
}

void Show::reset_mainField2(void)
{
  if(mainField2)  delete mainField2;
  mainField2=0;
}

const std::string* Show::get_mainField3(void)
{
  return mainField3;
}

bool Show::set_mainField3(const std::string& mainField3_)
{
  if(mainField3)  delete mainField3;
  mainField3=new std::string(mainField3_);
  return true;
}

void Show::reset_mainField3(void)
{
  if(mainField3)  delete mainField3;
  mainField3=0;
}

const std::string* Show::get_mainField4(void)
{
  return mainField4;
}

bool Show::set_mainField4(const std::string& mainField4_)
{
  if(mainField4)  delete mainField4;
  mainField4=new std::string(mainField4_);
  return true;
}

void Show::reset_mainField4(void)
{
  if(mainField4)  delete mainField4;
  mainField4=0;
}

const NsSmartDeviceLinkRPCV2::TextAlignment* Show::get_alignment(void)
{
  return alignment;
}

bool Show::set_alignment(const NsSmartDeviceLinkRPCV2::TextAlignment& alignment_)
{
  if(alignment)  delete alignment;
  alignment=new NsSmartDeviceLinkRPCV2::TextAlignment(alignment_);
  return true;
}

void Show::reset_alignment(void)
{
  if(alignment)  delete alignment;
  alignment=0;
}

const std::string* Show::get_statusBar(void)
{
  return statusBar;
}

bool Show::set_statusBar(const std::string& statusBar_)
{
  if(statusBar)  delete statusBar;
  statusBar=new std::string(statusBar_);
  return true;
}

void Show::reset_statusBar(void)
{
  if(statusBar)  delete statusBar;
  statusBar=0;
}

const std::string* Show::get_mediaClock(void)
{
  return mediaClock;
}

bool Show::set_mediaClock(const std::string& mediaClock_)
{
  if(mediaClock)  delete mediaClock;
  mediaClock=new std::string(mediaClock_);
  return true;
}

void Show::reset_mediaClock(void)
{
  if(mediaClock)  delete mediaClock;
  mediaClock=0;
}

const std::string* Show::get_mediaTrack(void)
{
  return mediaTrack;
}

bool Show::set_mediaTrack(const std::string& mediaTrack_)
{
  if(mediaTrack)  delete mediaTrack;
  mediaTrack=new std::string(mediaTrack_);
  return true;
}

void Show::reset_mediaTrack(void)
{
  if(mediaTrack)  delete mediaTrack;
  mediaTrack=0;
}

const NsSmartDeviceLinkRPCV2::Image* Show::get_graphic(void)
{
  return graphic;
}

bool Show::set_graphic(const NsSmartDeviceLinkRPCV2::Image& graphic_)
{
  if(graphic)  delete graphic;
  graphic=new NsSmartDeviceLinkRPCV2::Image(graphic_);
  return true;
}

void Show::reset_graphic(void)
{
  if(graphic)  delete graphic;
  graphic=0;
}

const std::vector< NsSmartDeviceLinkRPCV2::SoftButton>* Show::get_softButtons(void)
{
  return softButtons;
}

bool Show::set_softButtons(const std::vector< NsSmartDeviceLinkRPCV2::SoftButton>& softButtons_)
{
  if(softButtons)  delete softButtons;
  softButtons=new std::vector< NsSmartDeviceLinkRPCV2::SoftButton>(softButtons_);
  return true;
}

void Show::reset_softButtons(void)
{
  if(softButtons)  delete softButtons;
  softButtons=0;
}

const std::vector< std::string>* Show::get_customPresets(void)
{
  return customPresets;
}

bool Show::set_customPresets(const std::vector< std::string>& customPresets_)
{
  if(customPresets)  delete customPresets;
  customPresets=new std::vector< std::string>(customPresets_);
  return true;
}

void Show::reset_customPresets(void)
{
  if(customPresets)  delete customPresets;
  customPresets=0;
}

int Show::get_appId(void)
{
  return appId;
}

bool Show::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool Show::checkIntegrity(void)
{
  return ShowMarshaller::checkIntegrity(*this);
}
