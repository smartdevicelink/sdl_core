#include "../include/JSONHandler/ALRPCObjects/Image.h"
#include "ImageMarshaller.h"
#include "ImageTypeMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/



using namespace NsAppLinkRPC;

Image::Image(const Image& c)
{
  *this=c;
}


bool Image::checkIntegrity(void)
{
  return ImageMarshaller::checkIntegrity(*this);
}


Image::Image(void)
{
}



bool Image::set_imageType(const ImageType& imageType_)
{
  if(!ImageTypeMarshaller::checkIntegrityConst(imageType_))   return false;
  imageType=imageType_;
  return true;
}

bool Image::set_value(const std::string& value_)
{
  if(value_.length()>65535)  return false;
  value=value_;
  return true;
}




const ImageType& Image::get_imageType(void) const 
{
  return imageType;
}


const std::string& Image::get_value(void) const 
{
  return value;
}


