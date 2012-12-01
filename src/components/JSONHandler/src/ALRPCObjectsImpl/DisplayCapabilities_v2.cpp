#include "../include/JSONHandler/ALRPCObjects/DisplayCapabilities_v2.h"
#include "DisplayCapabilities_v2Marshaller.h"
#include "DisplayTypeMarshaller.h"
#include "MediaClockFormatMarshaller.h"
#include "TextField_v2Marshaller.h"
/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/



using namespace NsAppLinkRPC;

DisplayCapabilities_v2::DisplayCapabilities_v2(const DisplayCapabilities_v2& c)
{
  *this=c;
}


bool DisplayCapabilities_v2::checkIntegrity(void)
{
  return DisplayCapabilities_v2Marshaller::checkIntegrity(*this);
}


DisplayCapabilities_v2::DisplayCapabilities_v2(void)
{
}



bool DisplayCapabilities_v2::set_displayType(const DisplayType& displayType_)
{
  if(!DisplayTypeMarshaller::checkIntegrityConst(displayType_))   return false;
  displayType=displayType_;
  return true;
}

bool DisplayCapabilities_v2::set_mediaClockFormats(const std::vector<MediaClockFormat>& mediaClockFormats_)
{
  unsigned int i=mediaClockFormats_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!MediaClockFormatMarshaller::checkIntegrityConst(mediaClockFormats_[i]))   return false;
  }
  mediaClockFormats=mediaClockFormats_;
  return true;
}

bool DisplayCapabilities_v2::set_textFields(const std::vector<TextField_v2>& textFields_)
{
  unsigned int i=textFields_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!TextField_v2Marshaller::checkIntegrityConst(textFields_[i]))   return false;
  }
  textFields=textFields_;
  return true;
}




const DisplayType& DisplayCapabilities_v2::get_displayType(void) const 
{
  return displayType;
}


const std::vector<MediaClockFormat>& DisplayCapabilities_v2::get_mediaClockFormats(void) const 
{
  return mediaClockFormats;
}


const std::vector<TextField_v2>& DisplayCapabilities_v2::get_textFields(void) const 
{
  return textFields;
}


