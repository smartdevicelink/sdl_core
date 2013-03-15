//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/SoftButton.h"
#include "SoftButtonMarshaller.h"
#include "ImageMarshaller.h"
#include "SoftButtonTypeMarshaller.h"
#include "SystemActionMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/



using namespace NsSmartDeviceLinkRPCV2;

SoftButton& SoftButton::operator =(const SoftButton& c)
{
  image=c.image ? new Image(c.image[0]) : 0;
  isHighlighted=c.isHighlighted;
  softButtonID=c.softButtonID;
  systemAction=c.systemAction;
  text=c.text ? new std::string(c.text[0]) : 0;
  type=c.type;

  return *this;
}


SoftButton::~SoftButton(void)
{
  if(image)
    delete image;
  if(text)
    delete text;
}


SoftButton::SoftButton(const SoftButton& c)
{
  *this=c;
}


bool SoftButton::checkIntegrity(void)
{
  return SoftButtonMarshaller::checkIntegrity(*this);
}


SoftButton::SoftButton(void) :
    image(0),
    text(0)
{
}



bool SoftButton::set_image(const Image& image_)
{
  if(!ImageMarshaller::checkIntegrityConst(image_))   return false;
  delete image;
  image=0;

  image=new Image(image_);
  return true;
}

void SoftButton::reset_image(void)
{
  if(image)
    delete image;
  image=0;
}

bool SoftButton::set_isHighlighted(bool isHighlighted_)
{
  isHighlighted=isHighlighted_;
  return true;
}

bool SoftButton::set_softButtonID(unsigned int softButtonID_)
{
  if(softButtonID_>65536)  return false;
  softButtonID=softButtonID_;
  return true;
}

bool SoftButton::set_systemAction(const SystemAction& systemAction_)
{
  if(!SystemActionMarshaller::checkIntegrityConst(systemAction_))   return false;
  systemAction=systemAction_;
  return true;
}

bool SoftButton::set_text(const std::string& text_)
{
  if(text_.length()>500)  return false;
  delete text;
  text=0;

  text=new std::string(text_);
  return true;
}

void SoftButton::reset_text(void)
{
  if(text)
    delete text;
  text=0;
}

bool SoftButton::set_type(const SoftButtonType& type_)
{
  if(!SoftButtonTypeMarshaller::checkIntegrityConst(type_))   return false;
  type=type_;
  return true;
}




const Image* SoftButton::get_image(void) const 
{
  return image;
}


bool SoftButton::get_isHighlighted(void) const
{
  return isHighlighted;
}


unsigned int SoftButton::get_softButtonID(void) const
{
  return softButtonID;
}


const SystemAction& SoftButton::get_systemAction(void) const 
{
  return systemAction;
}


const std::string* SoftButton::get_text(void) const 
{
  return text;
}


const SoftButtonType& SoftButton::get_type(void) const 
{
  return type;
}


