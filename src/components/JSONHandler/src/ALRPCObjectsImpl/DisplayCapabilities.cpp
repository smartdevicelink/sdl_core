#include "../../include/JSONHandler/ALRPCObjects/DisplayCapabilities.h"
#include "DisplayCapabilitiesMarshaller.h"
#include "DisplayTypeMarshaller.h"
#include "MediaClockFormatMarshaller.h"
#include "TextFieldMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
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


