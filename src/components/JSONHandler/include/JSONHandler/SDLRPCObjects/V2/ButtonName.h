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

#ifndef NSSMARTDEVICELINKKRPCV2_BUTTONNAME_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_BUTTONNAME_INCLUDE


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

///  Defines the hard (physical) and soft (touchscreen) buttons available from SYNC

  class ButtonName
  {
  public:
    enum ButtonNameInternal
    {
      INVALID_ENUM=-1,
      OK=0,
      SEEKLEFT=1,
      SEEKRIGHT=2,
      TUNEUP=3,
      TUNEDOWN=4,
      PRESET_0=5,
      PRESET_1=6,
      PRESET_2=7,
      PRESET_3=8,
      PRESET_4=9,
      PRESET_5=10,
      PRESET_6=11,
      PRESET_7=12,
      PRESET_8=13,
      PRESET_9=14,
      CUSTOM_BUTTON=15
    };
  
    ButtonName() : mInternal(INVALID_ENUM)				{}
    ButtonName(ButtonNameInternal e) : mInternal(e)		{}
  
    ButtonNameInternal get(void) const	{ return mInternal; }
    void set(ButtonNameInternal e)		{ mInternal=e; }
  
  private:
    ButtonNameInternal mInternal;
    friend class ButtonNameMarshaller;
  };
  
}

#endif
