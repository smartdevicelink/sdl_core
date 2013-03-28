//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#include "../include/JSONHandler/SDLRPCObjects/V2/DisplayCapabilities.h"
#include "DisplayCapabilitiesMarshaller.h"
#include "DisplayTypeMarshaller.h"
#include "MediaClockFormatMarshaller.h"
#include "TextFieldMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/



using namespace NsSmartDeviceLinkRPCV2;

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


