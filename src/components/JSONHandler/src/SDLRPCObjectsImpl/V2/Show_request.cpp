//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/Show_request.h"
#include "Show_requestMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Marshaller.h"
#include "ImageMarshaller.h"
#include "SoftButtonMarshaller.h"
#include "TextAlignmentMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;
Show_request& Show_request::operator =(const Show_request& c)
{
  mainField1= c.mainField1 ? new std::string(c.mainField1[0]) : 0;
  mainField2= c.mainField2 ? new std::string(c.mainField2[0]) : 0;
  mainField3= c.mainField3 ? new std::string(c.mainField3[0]) : 0;
  mainField4= c.mainField4 ? new std::string(c.mainField4[0]) : 0;
  alignment= c.alignment ? new TextAlignment(c.alignment[0]) : 0;
  statusBar= c.statusBar ? new std::string(c.statusBar[0]) : 0;
  mediaClock= c.mediaClock ? new std::string(c.mediaClock[0]) : 0;
  mediaTrack= c.mediaTrack ? new std::string(c.mediaTrack[0]) : 0;
  graphic= c.graphic ? new Image(c.graphic[0]) : 0;
  softButtons= c.softButtons ? new std::vector<SoftButton>(c.softButtons[0]) : 0;
  customPresets= c.customPresets ? new std::vector<std::string>(c.customPresets[0]) : 0;

  return *this;
}


Show_request::~Show_request(void)
{
  if(mainField1)
    delete mainField1;
  if(mainField2)
    delete mainField2;
  if(mainField3)
    delete mainField3;
  if(mainField4)
    delete mainField4;
  if(alignment)
    delete alignment;
  if(statusBar)
    delete statusBar;
  if(mediaClock)
    delete mediaClock;
  if(mediaTrack)
    delete mediaTrack;
  if(graphic)
    delete graphic;
  if(softButtons)
    delete softButtons;
  if(customPresets)
    delete customPresets;
}


Show_request::Show_request(const Show_request& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool Show_request::checkIntegrity(void)
{
  return Show_requestMarshaller::checkIntegrity(*this);
}


Show_request::Show_request(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION),
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



bool Show_request::set_mainField1(const std::string& mainField1_)
{
  if(mainField1_.length()>500)  return false;
  delete mainField1;
  mainField1=0;

  mainField1=new std::string(mainField1_);
  return true;
}

void Show_request::reset_mainField1(void)
{
  if(mainField1)
    delete mainField1;
  mainField1=0;
}

bool Show_request::set_mainField2(const std::string& mainField2_)
{
  if(mainField2_.length()>500)  return false;
  delete mainField2;
  mainField2=0;

  mainField2=new std::string(mainField2_);
  return true;
}

void Show_request::reset_mainField2(void)
{
  if(mainField2)
    delete mainField2;
  mainField2=0;
}

bool Show_request::set_mainField3(const std::string& mainField3_)
{
  if(mainField3_.length()>500)  return false;
  delete mainField3;
  mainField3=0;

  mainField3=new std::string(mainField3_);
  return true;
}

void Show_request::reset_mainField3(void)
{
  if(mainField3)
    delete mainField3;
  mainField3=0;
}

bool Show_request::set_mainField4(const std::string& mainField4_)
{
  if(mainField4_.length()>500)  return false;
  delete mainField4;
  mainField4=0;

  mainField4=new std::string(mainField4_);
  return true;
}

void Show_request::reset_mainField4(void)
{
  if(mainField4)
    delete mainField4;
  mainField4=0;
}

bool Show_request::set_alignment(const TextAlignment& alignment_)
{
  if(!TextAlignmentMarshaller::checkIntegrityConst(alignment_))   return false;
  delete alignment;
  alignment=0;

  alignment=new TextAlignment(alignment_);
  return true;
}

void Show_request::reset_alignment(void)
{
  if(alignment)
    delete alignment;
  alignment=0;
}

bool Show_request::set_statusBar(const std::string& statusBar_)
{
  if(statusBar_.length()>500)  return false;
  delete statusBar;
  statusBar=0;

  statusBar=new std::string(statusBar_);
  return true;
}

void Show_request::reset_statusBar(void)
{
  if(statusBar)
    delete statusBar;
  statusBar=0;
}

bool Show_request::set_mediaClock(const std::string& mediaClock_)
{
  if(mediaClock_.length()>500)  return false;
  delete mediaClock;
  mediaClock=0;

  mediaClock=new std::string(mediaClock_);
  return true;
}

void Show_request::reset_mediaClock(void)
{
  if(mediaClock)
    delete mediaClock;
  mediaClock=0;
}

bool Show_request::set_mediaTrack(const std::string& mediaTrack_)
{
  if(mediaTrack_.length()>500)  return false;
  delete mediaTrack;
  mediaTrack=0;

  mediaTrack=new std::string(mediaTrack_);
  return true;
}

void Show_request::reset_mediaTrack(void)
{
  if(mediaTrack)
    delete mediaTrack;
  mediaTrack=0;
}

bool Show_request::set_graphic(const Image& graphic_)
{
  if(!ImageMarshaller::checkIntegrityConst(graphic_))   return false;
  delete graphic;
  graphic=0;

  graphic=new Image(graphic_);
  return true;
}

void Show_request::reset_graphic(void)
{
  if(graphic)
    delete graphic;
  graphic=0;
}

bool Show_request::set_softButtons(const std::vector<SoftButton>& softButtons_)
{
  unsigned int i=softButtons_.size();
  if(i>8 || i<0)  return false;
  while(i--)
  {
    if(!SoftButtonMarshaller::checkIntegrityConst(softButtons_[i]))   return false;
  }
  delete softButtons;
  softButtons=0;

  softButtons=new std::vector<SoftButton>(softButtons_);
  return true;
}

void Show_request::reset_softButtons(void)
{
  if(softButtons)
    delete softButtons;
  softButtons=0;
}

bool Show_request::set_customPresets(const std::vector<std::string>& customPresets_)
{
  unsigned int i=customPresets_.size();
  if(i>6 || i<0)  return false;
  while(i--)
  {
    if(customPresets_[i].length()>500)  return false;
  }
  delete customPresets;
  customPresets=0;

  customPresets=new std::vector<std::string>(customPresets_);
  return true;
}

void Show_request::reset_customPresets(void)
{
  if(customPresets)
    delete customPresets;
  customPresets=0;
}




const std::string* Show_request::get_mainField1(void) const 
{
  return mainField1;
}

const std::string* Show_request::get_mainField2(void) const 
{
  return mainField2;
}

const std::string* Show_request::get_mainField3(void) const 
{
  return mainField3;
}

const std::string* Show_request::get_mainField4(void) const 
{
  return mainField4;
}

const TextAlignment* Show_request::get_alignment(void) const 
{
  return alignment;
}

const std::string* Show_request::get_statusBar(void) const 
{
  return statusBar;
}

const std::string* Show_request::get_mediaClock(void) const 
{
  return mediaClock;
}

const std::string* Show_request::get_mediaTrack(void) const 
{
  return mediaTrack;
}

const Image* Show_request::get_graphic(void) const 
{
  return graphic;
}

const std::vector<SoftButton>* Show_request::get_softButtons(void) const 
{
  return softButtons;
}

const std::vector<std::string>* Show_request::get_customPresets(void) const 
{
  return customPresets;
}

