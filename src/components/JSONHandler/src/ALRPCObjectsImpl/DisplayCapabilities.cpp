#include "../../include/JSONHandler/ALRPCObjects/DisplayCapabilities.h"
#include "DisplayCapabilitiesMarshaller.h"
#include "DisplayTypeMarshaller.h"
#include "MediaClockFormatMarshaller.h"
#include "TextFieldMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/



DisplayCapabilities::DisplayCapabilities(const DisplayCapabilities& c)
{
  *this=c;
}


bool DisplayCapabilities::checkIntegrity(void)
{
  return DisplayCapabilitiesMarshaller::checkIntegrity(*this);
}


DisplayCapabilities::DisplayCapabilities(void)
{
}



bool DisplayCapabilities::set_displayType(const DisplayType& displayType_)
{
  if(!DisplayTypeMarshaller::checkIntegrityConst(displayType_))   return false;
  displayType=displayType_;
  return true;
}

bool DisplayCapabilities::set_mediaClockFormats(const std::vector<MediaClockFormat>& mediaClockFormats_)
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

bool DisplayCapabilities::set_textFields(const std::vector<TextField>& textFields_)
{
  unsigned int i=textFields_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!TextFieldMarshaller::checkIntegrityConst(textFields_[i]))   return false;
  }
  textFields=textFields_;
  return true;
}




const DisplayType& DisplayCapabilities::get_displayType(void) const 
{
  return displayType;
}


const std::vector<MediaClockFormat>& DisplayCapabilities::get_mediaClockFormats(void) const 
{
  return mediaClockFormats;
}


const std::vector<TextField>& DisplayCapabilities::get_textFields(void) const 
{
  return textFields;
}


