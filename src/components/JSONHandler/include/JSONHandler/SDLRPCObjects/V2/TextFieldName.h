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

#ifndef NSSMARTDEVICELINKKRPCV2_TEXTFIELDNAME_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_TEXTFIELDNAME_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPCV2
{

  class TextFieldName
  {
  public:
    enum TextFieldNameInternal
    {
      INVALID_ENUM=-1,

/**
     The first line of first set of main fields of the persistent display
     Applies to "Show"
*/
      mainField1=0,

/**
     The second line of first set of main fields of the persistent display
     Applies to "Show"
*/
      mainField2=1,

/**
     The first line of second set of main fields of persistent display
     Applies to "Show"
*/
      mainField3=2,

/**
     The second line of second set of main fields of the persistent display
     Applies to "Show"
*/
      mainField4=3,

/**
     The status bar on Nav
     Applies to "Show"
*/
      statusBar=4,

/**
     Text value for MediaClock field.
     Applies to "Show"
*/
      mediaClock=5,

/**
     The track field of NGN type ACMs.
     This field is only available for media applications.
     Applies to "Show"
*/
      mediaTrack=6,

/**
     The first line of the alert text field
     Applies to "Alert"
*/
      alertText1=7,

/**
     The second line of the alert text field
     Applies to "Alert"
*/
      alertText2=8,

/**
     The third line of the alert text field
     Applies to "Alert"
*/
      alertText3=9,

/**
      Long form body of text that can include newlines and tabs.
     Applies to "ScrollableMessage"
*/
      scrollableMessageBody=10,

///   First line suggestion for a user response (in the case of VR enabled interaction
      initialInteractionText=11,

///   First line of navigation text
      navigationText1=12,

///   Second line of navigation text
      navigationText2=13,

///   Estimated Time of Arrival time for navigation
      ETA=14,

///   Total distance to destination for navigation
      totalDistance=15,

///   First line of text for audio pass thru
      audioPassThruDisplayText1=16,

///   Second line of text for audio pass thru
      audioPassThruDisplayText2=17,

///   Header text for slider
      sliderHeader=18,

///   Footer text for slider
      sliderFooter=19
    };
  
    TextFieldName() : mInternal(INVALID_ENUM)				{}
    TextFieldName(TextFieldNameInternal e) : mInternal(e)		{}
  
    TextFieldNameInternal get(void) const	{ return mInternal; }
    void set(TextFieldNameInternal e)		{ mInternal=e; }
  
  private:
    TextFieldNameInternal mInternal;
    friend class TextFieldNameMarshaller;
  };
  
}

#endif
