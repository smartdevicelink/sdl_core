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

#ifndef NSSMARTDEVICELINKKRPCV2_SYSTEMACTION_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_SYSTEMACTION_INCLUDE


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

///  Enumeration that describes system actions that can be triggered.

  class SystemAction
  {
  public:
    enum SystemActionInternal
    {
      INVALID_ENUM=-1,

///  Default action occurs.  Standard behavior (e.g. SoftButton clears overlay).
      DEFAULT_ACTION=0,

///  App is brought into HMI_FULL.
      STEAL_FOCUS=1,

///  Current system context is maintained.  An overlay is persisted even though a SoftButton has been pressed and the notification sent.
      KEEP_CONTEXT=2
    };
  
    SystemAction() : mInternal(INVALID_ENUM)				{}
    SystemAction(SystemActionInternal e) : mInternal(e)		{}
  
    SystemActionInternal get(void) const	{ return mInternal; }
    void set(SystemActionInternal e)		{ mInternal=e; }
  
  private:
    SystemActionInternal mInternal;
    friend class SystemActionMarshaller;
  };
  
}

#endif
