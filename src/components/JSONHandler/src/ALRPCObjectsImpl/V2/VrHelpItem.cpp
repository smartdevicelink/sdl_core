#include "../include/JSONHandler/ALRPCObjects/V2/VrHelpItem.h"
#include "VrHelpItemMarshaller.h"
#include "ImageMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/



using namespace NsAppLinkRPCV2;

VrHelpItem::VrHelpItem(const VrHelpItem& c)
{
  *this=c;
}


bool VrHelpItem::checkIntegrity(void)
{
  return VrHelpItemMarshaller::checkIntegrity(*this);
}


VrHelpItem::VrHelpItem(void)
{
}



bool VrHelpItem::set_image(const Image& image_)
{
  if(!ImageMarshaller::checkIntegrityConst(image_))   return false;
  image=image_;
  return true;
}

bool VrHelpItem::set_position(unsigned int position_)
{
  if(position_>8)  return false;
  if(position_<1)  return false;
  position=position_;
  return true;
}

bool VrHelpItem::set_text(const std::string& text_)
{
  if(text_.length()>500)  return false;
  text=text_;
  return true;
}




const Image& VrHelpItem::get_image(void) const 
{
  return image;
}


unsigned int VrHelpItem::get_position(void) const
{
  return position;
}


const std::string& VrHelpItem::get_text(void) const 
{
  return text;
}


