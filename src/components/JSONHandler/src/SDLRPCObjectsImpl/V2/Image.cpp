//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/Image.h"
#include "ImageMarshaller.h"
#include "ImageTypeMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/



using namespace NsSmartDeviceLinkRPCV2;

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


