#include "../../include/JSONHandler/ALRPCObjects/Image.h"
#include "ImageMarshaller.h"
#include "ImageTypeMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/



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


