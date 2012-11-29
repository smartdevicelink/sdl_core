#include "../include/JSONHandler/ALRPCObjects/SoftButton.h"
#include "SoftButtonMarshaller.h"
#include "ImageMarshaller.h"
#include "SoftButtonTypeMarshaller.h"
#include "SystemActionMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/



using namespace NsAppLinkRPC;

SoftButton::SoftButton(const SoftButton& c)
{
  *this=c;
}


bool SoftButton::checkIntegrity(void)
{
  return SoftButtonMarshaller::checkIntegrity(*this);
}


SoftButton::SoftButton(void)
{
}



bool SoftButton::set_image(const Image& image_)
{
  if(!ImageMarshaller::checkIntegrityConst(image_))   return false;
  image=image_;
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




const Image& SoftButton::get_image(void) const 
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


