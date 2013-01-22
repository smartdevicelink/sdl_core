#include "../include/JSONHandler/ALRPCObjects/V2/SoftButton.h"
#include "SoftButtonMarshaller.h"
#include "ImageMarshaller.h"
#include "SoftButtonTypeMarshaller.h"
#include "SystemActionMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/


using namespace NsAppLinkRPCV2;

SoftButton::SoftButton(const SoftButton& c)
{
  this->isHighlighted = c.get_isHighlighted();
  this->softButtonID = c.get_softButtonID();
  this->systemAction = c.get_systemAction();
  this->text = c.get_text();
  this->type = c.get_type();

  this->image = 0;

  if (c.get_image())
    this->image = new Image(*(c.get_image()));
}

SoftButton& SoftButton::operator=(const SoftButton& c)
{
  this->isHighlighted = c.get_isHighlighted();
  this->softButtonID = c.get_softButtonID();
  this->systemAction = c.get_systemAction();
  this->text = c.get_text();
  this->type = c.get_type();

  this->image = 0;

  if (c.get_image())
    this->image = new Image(*(c.get_image()));

  return *this;
}


bool SoftButton::checkIntegrity(void)
{
  return SoftButtonMarshaller::checkIntegrity(*this);
}


SoftButton::SoftButton(void):image(0)
{
}



bool SoftButton::set_image(const Image& image_)
{
  if(!ImageMarshaller::checkIntegrityConst(image_))   return false;
  if (image)
  {
    delete image;
    image = 0;
  }
  image=new Image(image_);
  return true;
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
  text=text_;
  return true;
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


const std::string& SoftButton::get_text(void) const
{
  return text;
}


const SoftButtonType& SoftButton::get_type(void) const
{
  return type;
}


SoftButton::~SoftButton()
{
  delete image;
  image = 0;
}
