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

#ifndef NSSMARTDEVICELINKKRPC_SYNCMSGVERSION_INCLUDE
#define NSSMARTDEVICELINKKRPC_SYNCMSGVERSION_INCLUDE




/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/


namespace NsSmartDeviceLinkRPC
{

///  Specifies the version number of the SYNC V4 protocol, that is supported by the mobile application

  class SyncMsgVersion
  {
  public:
  
    SyncMsgVersion(const SyncMsgVersion& c);
    SyncMsgVersion(void);
  
    bool checkIntegrity(void);
  // getters

    unsigned int get_majorVersion(void) const;
    unsigned int get_minorVersion(void) const;

// setters

    bool set_majorVersion(unsigned int majorVersion_);
    bool set_minorVersion(unsigned int minorVersion_);

  private:

    friend class SyncMsgVersionMarshaller;


///  The major version indicates versions that is not-compatible to previous versions.
      unsigned int majorVersion;	//!<  (1,1)

///  The minor version indicates a change to a previous version that should still allow to be run on an older version (with limited functionality)
      unsigned int minorVersion;	//!<  (0,1000)
  };

}

#endif
